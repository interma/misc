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

std::atomic<int> id;

uint64_t gettid() { 
	return syscall(SYS_gettid); 
}

void hello(int i, int j){
	//std::cout << "Hello from thread:"  << i << " tid:" << std::this_thread::get_id() << std::endl; //not thread-safe
	printf("hello from thread:%d, tid1:%u, tid2:%llu\n", i, std::this_thread::get_id(), gettid());	
	std::this_thread::sleep_for(std::chrono::seconds(10));
}

int main(int argc, char **argv) {
    std::vector<std::thread> threads;

    for(int i = 0; i < 5; ++i){
        threads.push_back(std::thread(hello, i, i));
    }

    for(auto& thread : threads){
        thread.join();
    }

	std::cout<<"main exit"<<std::endl;
    return 0;
}



