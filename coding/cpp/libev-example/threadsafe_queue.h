/*
 desc: thread-safe queue
 author: interma (interma@outlook.com)
 created: 06/13/16 11:21:54 HKT
*/

#pragma once
#include <thread>
#include <mutex>
#include <chrono>
#include <iostream>
#include <list>
#include <condition_variable>

/*
struct noncopyable {
	noncopyable() {};
	noncopyable(const noncopyable&) = delete;
	noncopyable& operator=(const noncopyable&) = delete;
};
*/

/**
 * thread-safe queue
 */
//template<typename T> class TsQueue: private noncopyable {
template<typename T> class TsQueue {
public:
    bool push_back(T& v);
    T pop_front();

    size_t size();
private: 
    std::list<T> items_;
	std::mutex lock_;
	std::condition_variable not_empty_;
    //std::atomic<bool> exit_;
};

template<typename T> size_t TsQueue<T>::size() {
    std::lock_guard<std::mutex> lock(lock_);
    return items_.size();
}

template<typename T> bool TsQueue<T>::push_back(T& v) {
    std::unique_lock<std::mutex> lock(lock_);
    items_.push_back(v);
	not_empty_.notify_one();
    return true;
}

template<typename T> T TsQueue<T>::pop_front() {
    std::unique_lock<std::mutex> lock(lock_);
	not_empty_.wait(lock, [this]() {return items_.size() > 0; });
	T r = items_.front();
	items_.pop_front();
	return r;
}

