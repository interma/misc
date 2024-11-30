/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 06/09/16 20:35:59 HKT
*/
#pragma once

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
#include "rlsvr.h"


class RLConnection{
public:
    int fd;
    RLServer *server;
    struct ev_loop* loop;
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

	RLConnection(RLServer *s, int fd, struct ev_loop* l=NULL);
	~RLConnection();

    static void on_action(struct ev_loop *loop, ev_io *watcher, int revents);
    void start();

};


