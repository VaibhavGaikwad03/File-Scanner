#include "Thread_Pool.h"

ThreadPool::ThreadPool(unsigned int nums_threads) : num_thread(nums_threads), stop(false)
{
    for (int i = 0; i < num_thread; i++)
    {
        workers.emplace_back([this]
                             {

                while (true)
                {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(mtx);

                        cv.wait(lock, [this]{
                            return (!tasks.empty() || stop); 
                        });

                        if (stop) {return;}

                        task = move(tasks.front());
                        
                        tasks.pop();
                    }
                    task();

                    {
                        std::unique_lock<std::mutex> lock(complete_mutex);
                        if (tasks.empty())
                            complete_condition.notify_all();
                    }

                } });
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(mtx);
        stop = true;
    }

    cv.notify_all();

    for (std::thread &worker : workers)
        worker.join();
}

void ThreadPool::wait()
{
    std::unique_lock<std::mutex> lock(complete_mutex);
    complete_condition.wait(lock, [this]
                            { return tasks.empty(); });
}