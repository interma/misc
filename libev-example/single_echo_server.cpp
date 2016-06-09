/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 06/07/16 14:35:23 HKT
*/

//single thread, echo server
//use telnet as client
//use context mode

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/tcp.h> /* TCP_NODELAY */
#include <netinet/in.h>  /* inet_ntoa */
#include <arpa/inet.h>   /* inet_ntoa */

#include <ev.h>
#include <string>

void set_nonblock (int fd) {
	int flags = fcntl(fd, F_GETFL, 0);
	int r = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	assert(0 <= r && "Setting socket non-block failed!");
}

class RLServer;
class RLConnection{
public:
    int fd;
    RLServer *server;
	enum AState { //action state
		BEG,
		READ,
		WRITE
	};	
	AState state;

    char read_buffer[1024];
    std::string write_buffer;

    ev_io action_watcher;
    //ev_timer timeout_watcher; //todo

	RLConnection(RLServer *s, int fd) :fd(fd), server(s), state(BEG)
	{
		set_nonblock(fd);

		ev_init(&action_watcher, RLConnection::on_action);
		action_watcher.data = this;
		
		//timeout_watcher.data = this;

		//server->clients_num++;
	}
	~RLConnection()
	{
		if (state != BEG){
			ev_io_stop(server->loop, &action_watcher);
			close(fd);
		}
		//server->clients_num--;
	}
    static void on_action(struct ev_loop *loop, ev_io *watcher, int revents);
    void start();

};


class RLServer{
public:
    int port;
    int fd;
    int clients_num;
    struct ev_loop* loop;
    ev_io connection_watcher;

    RLServer() : port(1220), fd(-1), clients_num(0) 
	{
		loop = ev_loop_new(EVFLAG_AUTO);
		connection_watcher.data = this;
	}
    ~RLServer()
	{
		if (loop) 
			ev_loop_destroy(loop);
		close(fd);
	}
    void start()
	{
		struct linger ling = {0, 0};
		struct sockaddr_in addr;
		int flags = 1;

		if((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			perror("socket()");
			exit(1);
		}

		flags = 1;
		setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void *)&flags, sizeof(flags));
		setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (void *)&flags, sizeof(flags));
		setsockopt(fd, SOL_SOCKET, SO_LINGER, (void *)&ling, sizeof(ling));
		setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (void *)&flags, sizeof(flags));

		memset(&addr, 0, sizeof(addr));

		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = htonl(INADDR_ANY);

		if(bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
			perror("bind()");
			if(fd > 0) close(fd);
			exit(1);
		}

		if(listen(fd, 1024) < 0) {
			perror("listen()");
			exit(1);
		}

		set_nonblock(fd);

		ev_init(&connection_watcher, RLServer::on_connection);

		ev_io_set(&connection_watcher, fd, EV_READ);
		ev_io_start(loop, &connection_watcher);

		printf("Server running successfully\n");
		ev_run(loop, 0);
	}

    static void on_connection(struct ev_loop *loop, ev_io *watcher, int revents) 
	{
		RLServer *s = static_cast<RLServer*>(watcher->data);
		if(EV_ERROR & revents) {
			puts("on_connection() got error event, closing server.");
			return;
		}

		struct sockaddr_in addr; // connector's address information
		socklen_t addr_len = sizeof(addr);
		int fd = accept(s->fd, (struct sockaddr*)&addr, &addr_len);

		if(fd < 0) {
			perror("accept()");
			return;
		}

		printf("fd[%d] connected\n", fd);
		//when delete: loop read==0
		RLConnection *connection = new RLConnection(s, fd); 

		if(connection == NULL) {
			close(fd);
			return;
		}
		connection->start();
	}

};

/**
 * one read, one write action mode
 */ 
void RLConnection::on_action(struct ev_loop *loop, ev_io *watcher, int revents)
{
	//puts("i am in action");
	RLConnection *connection = static_cast<RLConnection*>(watcher->data);
	if(EV_ERROR & revents) {
		puts("on_readable() got error event, closing connection.");
		return;
	}
	int fd = connection->fd;
	int state = connection->state;
	printf("in action, state[%d]\n", state);

	ev_io *action_watcher = &(connection->action_watcher);
	char *read_buffer = connection->read_buffer;
	std::string &write_buffer = connection->write_buffer;
	RLServer *server = connection->server;
	
	if (state == BEG || state == READ) {
		int cnt = read(fd, read_buffer, sizeof(connection->read_buffer));
		if (cnt == 0) //peer close
		{
			printf("fd[%d] disconnected\n", fd);
			//ev_io_stop(server->loop, action_watcher);
			//close(fd);
			delete connection;
			return;
		}
		write_buffer.clear();
		char msg[] = "i give you:";
		write_buffer.append(msg, sizeof(msg));
		write_buffer.append(read_buffer, cnt);

		connection->state = WRITE;
		ev_io_stop(server->loop, action_watcher);
		ev_io_set(action_watcher, fd, EV_WRITE);
		ev_io_start(server->loop, action_watcher);
	}
	else if (state == WRITE) {
		write(fd, write_buffer.data(), write_buffer.length());

		connection->state = READ;
		ev_io_stop(server->loop, action_watcher);
		ev_io_set(action_watcher, fd, EV_READ);
		ev_io_start(server->loop, action_watcher);
	}
	else {
		puts("unknown state");
		return;
	}
}

void RLConnection::start()
{
	ev_io_set(&action_watcher, fd, EV_READ);
	ev_io_start(server->loop, &action_watcher);
}	

int main(int argc, char **argv)
{
	RLServer server;
	server.start();
	printf("server exit\n");
	return 0;
}

