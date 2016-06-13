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
#include <thread>

#include "rlsvr.h"
#include "rlcon.h"

struct ev_loop** g_loop;
struct ev_async* g_async;
std::thread* g_threads;

static void thread_func(int i) {
	std::hash<std::thread::id> hasher;
	printf("i am thread[%d], tid[%u]\n", i, hasher(std::this_thread::get_id()));
	struct ev_loop * loop = g_loop[i];
	ev_run(loop, 0);
}

//called in loop thread
static void async_callback(struct ev_loop* loop, ev_async* watcher, int revents) {
	assert(loop);
	assert(watcher);
	assert(revents == EV_ASYNC);
	
	std::hash<std::thread::id> hasher;
	printf("async_callback tid[%u]\n", hasher(std::this_thread::get_id()));

}

int main(int argc, char **argv)
{
	bool mt = false; //multi_thread
	if (argc == 2) 
		mt = true;

	const int threads = 3; //worker threads
	if (mt) {
		
	 	g_loop = new struct ev_loop* [threads];
		g_async = new ev_async [threads];
		g_threads = new std::thread[threads];
		
		for(int i = 0; i < threads; i++) {
			//init thread loop
			g_loop[i] = ev_loop_new(EVFLAG_AUTO);
			//g_loop[i] = ev_loop_new(EVBACKEND_EPOLL);
			assert(g_loop[i] && "ev_loop_new failed!");
			//ev_ref(g_loop[i]);
			ev_async_init(g_async + i, async_callback);
			ev_async_start(g_loop[i], g_async + i);

			//start worker
			g_threads[i] = std::thread(thread_func,i);
			
		}	

	}

	RLServer server(mt?threads:0);
	server.start();
	printf("server exit\n");

	if (mt) {
		//clean muliti struct
		for(int i = 0; i < threads; i++) {
			g_threads[i].join();
		}
	}

	return 0;
}

