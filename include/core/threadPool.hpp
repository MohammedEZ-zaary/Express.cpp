#include <thread>
#include <vector>
#include <condition_variable>
#include <queue>
#include <mutex>
#include <functional>

class ThreadPool {
    public:
        ThreadPool(size_t num_threads) {
            for (size_t i = 0; i < num_threads; ++i) {
                workers.emplace_back([this] {
                    while (true) {
                        std::function<void()> task;
    
                        {
                            std::unique_lock<std::mutex> lock(queue_mutex);
                            condition.wait(lock, [this] { return !tasks.empty() || stop; });
                            if (stop && tasks.empty()) return;
                            task = std::move(tasks.front());  // Move the task from the queue
                            tasks.pop();
                        }
    
                        task();  // Execute the task
                    }
                });
            }
        }
    
        template <typename F>
        void enqueue(F&& task) {
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                tasks.emplace(std::forward<F>(task));  // Perfect forwarding to avoid copying
            }
            condition.notify_one();
        }
    
        ~ThreadPool() {
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                stop = true;
            }
            condition.notify_all();
            for (std::thread& worker : workers) {
                worker.join();
            }
        }
    
    private:
        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;
        std::mutex queue_mutex;
        std::condition_variable condition;
        bool stop = false;
    };