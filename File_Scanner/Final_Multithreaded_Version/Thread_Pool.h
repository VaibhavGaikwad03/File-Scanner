#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <iostream>
#include <thread>
#include <functional>
#include <queue>
#include <mutex>
#include <future>
#include <condition_variable>

class ThreadPool
{
    bool stop;
    std::mutex mtx;
    std::mutex complete_mutex;
    unsigned int num_thread;
    std::condition_variable cv;
    std::condition_variable complete_condition;
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

public:
    explicit ThreadPool(unsigned int nums_threads);
    ~ThreadPool();
    void wait();

    template <class F, class... Args>
    auto execute_task(F &&f, Args &&...args) -> std::future<decltype(f(args...))>
    {
        using return_type = decltype(f(args...));

        auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<return_type> result = task->get_future();

        {
            std::unique_lock<std::mutex> lock(mtx);

            if (stop)
                throw std::runtime_error("Enqueue on stopped ThreadPool");

            tasks.emplace([task]() -> void
                          { (*task)(); });
        }

        cv.notify_one();

        return result;
    }
};

#endif // THREAD_POOL_H