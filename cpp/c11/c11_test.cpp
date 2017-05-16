/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 06/09/16 21:16:16 HKT
*/

#include <stdio.h>
#include <atomic>
#include <thread>
#include <iostream>
#include <vector>
#include <chrono>

#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>

#include "threadsafe_queue.h"

std::atomic<int> id;

uint64_t gettid() { 
	return syscall(SYS_gettid); 
}

//==========

void hello(int i, int j){
	//std::cout << "Hello from thread:"  << i << " tid:" << std::this_thread::get_id() << std::endl; //not thread-safe
	printf("hello from thread:%d, tid1:%u, tid2:%llu\n", i, std::this_thread::get_id(), gettid());	
	//std::this_thread::sleep_for(std::chrono::seconds(10));
}
void test_thread_tid() {
    std::vector<std::thread> threads;

    for(int i = 0; i < 5; ++i){
        threads.push_back(std::thread(hello, i, i));
    }

    for(auto& thread : threads){
        thread.join();
    }

}

void cosumer(TsQueue<int> &q) {
	std::hash<std::thread::id> hasher;
	for (int i = 0; i < 10; i++) {
		int r = q.pop_front();
		printf("thread[%u] get value:%d\n",hasher(std::this_thread::get_id()),r);
	}
}
void producer(TsQueue<int> &q) {
	std::this_thread::sleep_for(std::chrono::seconds(3));
	for (int i = 0; i < 50; i++) {
		q.push_back(i);
	}
}
void test_queue(){
	TsQueue<int> q;
    std::thread t_c1(cosumer, std::ref(q));
    std::thread t_c2(cosumer, std::ref(q));
    std::thread t_p1(producer,std::ref(q));

	t_c1.join();
	t_c2.join();
	t_p1.join();
}



int main(int argc, char **argv) {

	test_thread_tid();
	test_queue();
	std::cout<<"main exit"<<std::endl;
    return 0;
}



