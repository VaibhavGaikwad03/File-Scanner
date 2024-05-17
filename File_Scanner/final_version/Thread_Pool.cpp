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
    unsigned int num_thread;
    std::condition_variable cv;
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;

public:
    explicit ThreadPool(unsigned int nums_threads) : num_thread(nums_threads), stop(false)
    {
        for (int i = 0; i < num_thread; i++)
        {
            threads.emplace_back([this]
                                 {
            std::function<void()> task;

                while (1)
                {
                    std::unique_lock<std::mutex> lock(mtx);

                    cv.wait(lock, [this]{
                        return (!tasks.empty() || stop); 
                    });

                    if (stop) {return;}

                    task = move(tasks.front());
                    
                    tasks.pop();

                    std::cout << "Size is: " << tasks.size() << std::endl;

                    lock.unlock();

                    task();
                } });
        }
    }

    ~ThreadPool()
    {
        std::unique_lock<std::mutex> lock(mtx);
        stop = true;

        lock.unlock();
        cv.notify_all();

        for (auto &t : threads)
            t.join();
    }

    template <class F, class... Args>
    auto execute_task(F &&f, Args &&...args) -> std::future<decltype(f(args...))>
    {
        using return_type = decltype(f(args...));

        auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<return_type> res = task->get_future();

        std::unique_lock<std::mutex> lock(mtx);

        tasks.emplace([task]() -> void
                      { (*task)(); });

        lock.unlock();

        cv.notify_one();

        return res;
    }
};

int task_func(int a)
{
    std::cout << "Calculating a * a ";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return a * a;
}

int main()
{
    ThreadPool pool(std::thread::hardware_concurrency());

    while (1)
    {
        std::future<int> res = pool.execute_task(task_func, 2);
        std::cout << "Result is: " << res.get() << std::endl;
    }

    return 0;
}