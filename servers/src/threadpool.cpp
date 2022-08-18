#include "threadpool.hpp"

HDE::ThreadPool::ThreadPool(int count) : cnt(count), running_flag(false)
{
}
HDE::ThreadPool::~ThreadPool()
{
    stop();
}
void HDE::ThreadPool::start()
{
    // 启动线程
    running_flag = true;
    // 创建线程
    for (int i = 0; i < cnt; i++)
    {
        //构造就会执行work函数
        threads.emplace_back(&ThreadPool::work, this);
    }
}
void HDE::ThreadPool::stop()
{
    // 所有任务完成后结束
    while (true)
    {
        std::unique_lock<std::mutex> ul(mtx);
        if (tasks.empty())
            break;
        ul.unlock();
    }
    running_flag = false;
    // 唤醒所有线程
    cv.notify_all();
    // 等待所有任务全部结束
    for (auto &t : threads)
    {
        if (t.joinable())
            t.join(); //阻塞直至完成
    }
}

void HDE::ThreadPool::push_task(EventHandler *task)
{
    //新增clientsocket
    std::unique_lock<std::mutex> ul(this->mtx);
    tasks.push(task);
    ul.unlock();
    this->cv.notify_one();
}
void HDE::ThreadPool::work()
{
    while (true)
    {
        if (!this->running_flag)
            break;
        // 执行一个任务
        EventHandler *task = nullptr;
        std::unique_lock<std::mutex> ul(this->mtx);
        this->cv.wait(ul, [&]()
                      { return !tasks.empty() || !running_flag; });
        if (!running_flag)
            return;
        task = tasks.front();
        tasks.pop();
        ul.unlock();
        // 执行任务
        if (task != nullptr)
        {
            task->run();
            //释放内存
            delete task;
        }
    }
}