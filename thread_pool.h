//
// Created by Ashot on 5/12/2020.
//

#ifndef COLLISIONS_THREAD_POOL_H
#define COLLISIONS_THREAD_POOL_H

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
using namespace std;

#define MAX_THREADS std::thread::hardware_concurrency()

struct Task{
    static int id;
    int task_id;
    Task();
    virtual void exec(){};
    virtual ~Task();
};


struct ThreadPool{
    bool _n_threads;
    Task* ready_task;
    bool _pool_created = false;

    ThreadPool(int n_threads);
    void _create_pool();
    void add_task(Task * t);
    void infinite_loop();
    bool finished_task();
    void wait();
    void join();
    ~ThreadPool();
private:
    std::vector<std::thread> threads;
    queue<Task*> tasks;
    queue<Task*> finished;
    atomic<int> running_tasks;
    std::mutex mx_for_tasks;
    std::mutex mx_for_finished;
    std::mutex mx_for_waiter;
    std::condition_variable locker;
    std::condition_variable waiter;
    bool stop_threads = false;
};

#endif //COLLISIONS_THREAD_POOL_H
