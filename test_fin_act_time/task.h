#ifndef _TASK_H_
#define _TASK_H_
#include <functional>
#include <memory>
#include <condition_variable>
#include <queue>
#include <mutex>
#include <vector>
#include <thread>
#include <list>
namespace TASK {
typedef enum {
    E_REMOVE_TASK = 0,
    E_KEEP_TASK = 1
}E_TASK_RET;

class Task {
public:
    Task();
    Task(const std::string &task_id, std::function<E_TASK_RET(void*)> &&func, void *param);
    virtual ~Task();
    std::string getTaskId();
    virtual E_TASK_RET work();
    uint64_t start_time_;//开始执行时间
    uint64_t end_time_;//结束执行时间
protected:
    std::string task_id_;
    std::shared_ptr<std::function<E_TASK_RET(void*)>> task_func_;
    void *param_;
};

class TaskQueue {
public:
    TaskQueue(int id,size_t max_task_count, uint32_t run_interval_ms);
    virtual ~TaskQueue();
    void start();
    void stop();
    size_t taskCount();
    bool isFull();
    bool isEmpty();
    bool pushTask(std::shared_ptr<Task> task);
    std::shared_ptr<Task> searchTask(const std::string &task_id);
    bool removeTask(const std::string &task_id);
protected:
    std::mutex task_list_mutex_;
    size_t max_task_count_;
    uint32_t run_interval_ms_;
    std::condition_variable task_cv_;
    std::list<std::shared_ptr<Task>> task_list_;
    std::shared_ptr<std::thread> process_thread_;
    bool exit_;
    int id_;
};

class ThreadPoolTaskExecutor {
public:
    ThreadPoolTaskExecutor(size_t min_pool_size, size_t max_pool_size, size_t max_task_per_thread, uint32_t run_interval_ms);
    virtual ~ThreadPoolTaskExecutor();
    bool execute(std::shared_ptr<Task> task);
    bool pushTask(std::shared_ptr<Task> task);
    std::shared_ptr<Task> searchTask(const std::string &task_id);
    bool removeTask(const std::string &task_id);
protected:
    size_t min_pool_size_;//最小数量
    size_t max_pool_size_;//最大数量
    size_t max_task_per_thread_;
    uint32_t run_interval_ms_;

    std::mutex task_queues_mutex_;
    std::shared_ptr<TaskQueue> a;
    std::vector<std::shared_ptr<TaskQueue>> task_queues_;
    size_t index_;
};
};

#endif