#include <queue>
#include <mutex>
#include <condition_variable>

#include <functional>
#include <queue>

class TaskQueue {
public:
    using Task = std::function<void()>;
    bool finished_adding_tasks = false; // New flag

    void push(Task task) {
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push(std::move(task));
        condition_.notify_one();
    }

    Task pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [this] { return !queue_.empty() || finished_adding_tasks; });
        if(queue_.empty()) return nullptr; // Return nullptr if no tasks and finished adding
        Task task = std::move(queue_.front());
        queue_.pop();
        return task;
    }

    void finish() {
        std::unique_lock<std::mutex> lock(mutex_);
        finished_adding_tasks = true;
        condition_.notify_all(); // Wake up all waiting threads
    }   

private:
    mutable std::mutex mutex_;
    std::condition_variable condition_;
    std::queue<Task> queue_;
};
