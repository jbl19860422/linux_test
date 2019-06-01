#include "task.h"
#include <assert.h>
using namespace TASK;
Task::Task() {

}

Task::Task(const std::string &task_id, std::function<E_TASK_RET(void*)> &&func, void *param) {
    task_func_ = std::make_shared<std::function<E_TASK_RET(void*)>>(func);
    param_ = param;
    task_id_ = task_id;
}

E_TASK_RET Task::work() {
    //start_time_ = detail::get_milisecond();
    E_TASK_RET ret = (*task_func_)(param_);
    //end_time_ = detail::get_milisecond();
    return ret;
}

Task::~Task() {

}

std::string Task::getTaskId() {
    return task_id_;
}

TaskQueue::TaskQueue(int id, size_t max_task_count, uint32_t run_interval_ms):process_thread_(nullptr), exit_(false) {
    run_interval_ms_ = run_interval_ms;
    max_task_count_ = max_task_count;
    id_ = id;
}

void TaskQueue::start() {
    exit_ = false;
    process_thread_ = std::make_shared<std::thread>([=] {
            while(1) {
                std::list<std::shared_ptr<Task>> task_list_takeout;
                {
                    std::unique_lock<std::mutex> lck(task_list_mutex_);
                    if(task_cv_.wait_for(lck,std::chrono::milliseconds(run_interval_ms_)) == std::cv_status::timeout) {
                        if(task_list_.size() <= 0) {
                            continue;
                        }
                    }

                    if(exit_) {
                        break;
                    }
                    
                    if(task_list_.size() <= 0) {
                        continue;
                    }
                    task_list_takeout.swap(task_list_);
                }

                for(auto & task : task_list_takeout) {
                    if(E_KEEP_TASK == task->work()) {//如果是因为等待执行，重新入队列
                        pushTask(task);
                    } else {
                        task.reset();
                        task = nullptr;
                    }

                }
            }
        }
    );
}

TaskQueue::~TaskQueue() {
    stop();
}

void TaskQueue::stop() {
    if(!process_thread_) {
        return;
    }

    exit_ = true;
    task_cv_.notify_one();
    if(process_thread_->joinable()) {
        process_thread_->join();
    }

    std::unique_lock<std::mutex> lck(task_list_mutex_);
    while(!task_list_.empty()) {
        task_list_.pop_front();
    }
}

size_t TaskQueue::taskCount() {
    std::unique_lock<std::mutex> lck(task_list_mutex_);
    return task_list_.size();
}

bool TaskQueue::pushTask(std::shared_ptr<Task> task) {
    std::unique_lock<std::mutex> lck(task_list_mutex_);
    if(task_list_.size() >= max_task_count_) {
        return false;
    }
    task_list_.push_back(task);
    task_cv_.notify_one();
    return true;
}

std::shared_ptr<Task> TaskQueue::searchTask(const std::string &task_id) {
    std::unique_lock<std::mutex> lck(task_list_mutex_);
    for(auto &task : task_list_) {
        if(task->getTaskId() == task_id) {
            return task;
        }
    }
    return nullptr;
}

bool TaskQueue::removeTask(const std::string &task_id) {
    std::unique_lock<std::mutex> lck(task_list_mutex_);
    for(auto &task : task_list_) {
        if(task->getTaskId() == task_id) {
            task_list_.remove(task);
            return true;
        }
    }
    return false;
}

bool TaskQueue::isFull() {
    std::unique_lock<std::mutex> lck(task_list_mutex_);
    return task_list_.size() == max_task_count_;
}

bool TaskQueue::isEmpty() {
    std::unique_lock<std::mutex> lck(task_list_mutex_);
    return task_list_.size() == 0;
}

ThreadPoolTaskExecutor::ThreadPoolTaskExecutor(size_t min_pool_size, size_t max_pool_size, size_t max_task_per_thread, uint32_t run_interval_ms) {
    assert(min_pool_size > 0 && max_pool_size > 0 && min_pool_size <= max_pool_size);
    assert(max_task_per_thread > 0);
    index_ = 0;
    run_interval_ms_ = run_interval_ms;
    min_pool_size_ = min_pool_size;
    max_pool_size_ = max_pool_size;
    max_task_per_thread_ = max_task_per_thread;
    for(size_t i = 0; i < min_pool_size_; i++) {
        std::shared_ptr<TaskQueue> task_queue = std::make_shared<TaskQueue>(i, max_task_per_thread, run_interval_ms_);
        task_queue->start();
        task_queues_.push_back(task_queue);
    }
}

bool ThreadPoolTaskExecutor::execute(std::shared_ptr<Task> task) {
    //std::lock_guard<std::mutex> lck(task_queues_mutex_);
    if(task_queues_.size() <= 0) {
        return false;
    }
    //取到最小任务数量的队列
    std::shared_ptr<TaskQueue> task_queue;
    //task_queue = task_queues_[(index_++)%task_queues_.size()];
    task_queue = task_queues_[0];
    size_t min_task_count = task_queues_[0]->taskCount();
    for(size_t i = 1; i < task_queues_.size(); i++) {
        if(min_task_count > task_queues_[i]->taskCount()) {
            min_task_count = task_queues_[i]->taskCount();
            task_queue = task_queues_[i];
        }
    }
    //推送任务
    if(!task_queue->pushTask(task)) {//任务不够了
        if(task_queues_.size() >= max_pool_size_) {
            return false;
        }
        task_queue  = std::make_shared<TaskQueue>(task_queues_.size(), max_task_per_thread_, run_interval_ms_);
        task_queues_.push_back(task_queue);
        return task_queue->pushTask(task);
    }

    if(task_queues_.size() > min_pool_size_) {
        size_t need_remove_count = task_queues_.size() - min_pool_size_;
        for(std::vector<std::shared_ptr<TaskQueue>>::iterator it = task_queues_.begin(); it != task_queues_.end() && need_remove_count > 0;) {
            if((*it)->isEmpty()) {
                (*it)->stop();
                (*it).reset();
                it = task_queues_.erase(it);
                need_remove_count--;
            } else {
                it++;
            }
        }
    }
    return true;
}

std::shared_ptr<Task> ThreadPoolTaskExecutor::searchTask(const std::string &task_id) {
    for(auto &task_queue : task_queues_) {
        std::shared_ptr<Task> task = task_queue->searchTask(task_id);
        if(task) {
            return task;
        }
    }
    return nullptr;
}

bool ThreadPoolTaskExecutor::removeTask(const std::string &task_id) {
    for(auto &task_queue : task_queues_) {
        if(task_queue->removeTask(task_id)) {
            return true;
        }
    }
    return false;
}

ThreadPoolTaskExecutor::~ThreadPoolTaskExecutor() {
    //std::lock_guard<std::mutex> lck(task_queues_mutex_);
    for(size_t i = 0; i < task_queues_.size(); i++) {
        task_queues_[i]->stop();
        task_queues_[i].reset();
    }
    task_queues_.clear();
}