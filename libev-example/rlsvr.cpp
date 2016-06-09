/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 06/09/16 20:35:38 HKT
*/

#include "rlsvr.h"
#include "rlcon.h" 

void set_nonblock (int fd) {
	int flags = fcntl(fd, F_GETFL, 0);
	int r = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	assert(0 <= r && "Setting socket non-block failed!");
}


RLServer::RLServer() : port(1220), fd(-1), clients_num(0) 
{
	loop = ev_loop_new(EVFLAG_AUTO);
	connection_watcher.data = this;
}
RLServer::~RLServer()
{
	if (loop) 
		ev_loop_destroy(loop);
	close(fd);
}
void RLServer::start()
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

void RLServer::on_connection(struct ev_loop *loop, ev_io *watcher, int revents) 
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
