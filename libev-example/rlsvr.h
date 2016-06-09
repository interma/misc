/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 06/09/16 20:35:46 HKT
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

class RLServer{
public:
    int port;
    int fd;
    int clients_num;
    struct ev_loop* loop;
    ev_io connection_watcher;

    RLServer();
    ~RLServer();

    void start();
    static void on_connection(struct ev_loop *loop, ev_io *watcher, int revents);

};
