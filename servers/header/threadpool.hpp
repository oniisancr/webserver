#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__
#include "eventhandler.hpp"
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "simplepoller.hpp"
namespace HDE
{
    class ThreadPool
    {
    private:
        int cnt; //工作线程的数量
        std::vector<std::thread> threads;
        std::queue<EventHandler*> tasks;
        std::atomic_bool running_flag;
        std::condition_variable cv;
        std::mutex mtx;
        void work();

    public:
        ThreadPool(int count);
        ~ThreadPool();
        void start();
        void stop();
        void push_task(EventHandler*);
    };
}
#endif // __THREADPOOL_H__