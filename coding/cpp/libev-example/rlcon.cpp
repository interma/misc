/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 06/09/16 20:35:54 HKT
*/

#include <thread>
#include <chrono>
#include "rlcon.h"

RLConnection::RLConnection(RLServer *s, int fd, struct ev_loop* l) :fd(fd), server(s), state(BEG)
{
	//set_nonblock(fd);

	ev_init(&action_watcher, RLConnection::on_action);
	action_watcher.data = this;
	loop = l;
	if (loop == NULL)
		loop = s->loop;

	//timeout_watcher.data = this;

	//server->clients_num++;
}
RLConnection::~RLConnection()
{
	if (state != BEG){
		ev_io_stop(server->loop, &action_watcher);
		close(fd);
	}
	//server->clients_num--;
}

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

		//simulation slow action
		//std::this_thread::sleep_for(std::chrono::seconds(10));

		connection->state = WRITE;
		ev_io_stop(loop, action_watcher);
		ev_io_set(action_watcher, fd, EV_WRITE);
		ev_io_start(loop, action_watcher);
	}
	else if (state == WRITE) {
		write(fd, write_buffer.data(), write_buffer.length());

		connection->state = READ;
		ev_io_stop(loop, action_watcher);
		ev_io_set(action_watcher, fd, EV_READ);
		ev_io_start(loop, action_watcher);
	}
	else {
		puts("unknown state");
		return;
	}
}

void RLConnection::start()
{
	ev_io_set(&action_watcher, fd, EV_READ);
	ev_io_start(loop, &action_watcher);
}	
