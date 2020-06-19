//
// Created by Ashot on 5/12/2020.
//

#include "thread_pool.h"

Task::Task():task_id(++id){}
int Task::id = 0;

Task::~Task()=default;


ThreadPool::ThreadPool(int n_threads):_n_threads(n_threads),
                        ready_task(NULL),running_tasks(0){

};
void ThreadPool::_create_pool() {
    for (int i = 0; i < _n_threads; i++) {
        threads.push_back(
                std::thread(&ThreadPool::infinite_loop, this));
    }
    _pool_created = true;
}
void ThreadPool::add_task(Task* t){
    if (!_pool_created) {
        _create_pool();
    }
    tasks.push(t);
    locker.notify_one();
}
void ThreadPool::infinite_loop(){
    while(true){
        std::unique_lock<std::mutex> lock(mx_for_tasks);
        locker.wait(lock, [this] {
            return !tasks.empty() || stop_threads; });
        if (stop_threads){break;}
        Task& task = *tasks.front();tasks.pop();
        lock.unlock();
        running_tasks++;

        task.exec();
        std::unique_lock<std::mutex> lock2(mx_for_finished);
        finished.push(&task);
        lock2.unlock();
        running_tasks--;
        waiter.notify_all();
    }
}
bool ThreadPool::finished_task(){
    while(true){
        std::unique_lock<std::mutex> lock(mx_for_finished);
        if (!finished.empty()){
            delete  ready_task;
            ready_task = finished.front();finished.pop();
            return true;
        }
        else if(running_tasks==0 && tasks.empty()){
            return false;
        }
        waiter.wait(lock, [this] {
            return  running_tasks==0 || !finished.empty();});
    }
}
void ThreadPool::wait(){
    while(true){
        std::unique_lock<std::mutex> lock(mx_for_waiter);
        waiter.wait(lock, [this] {
            return running_tasks==0 && tasks.empty();});
        break;
    }
}
void ThreadPool::join(){
    stop_threads = true;
    locker.notify_all();

    for (auto& t : threads){
        t.join();
    }
}
ThreadPool::~ThreadPool(){
    join();
}