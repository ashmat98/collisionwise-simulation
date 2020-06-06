
#include "model.h"

struct CollideTask: Task{
    Model& model;
    int particle_id;
    int other_start;
    int other_finish;
    queue<TimeQueueObj> result;
    CollideTask(Model& model,
                int particle_id, int other_start, int other_finish):
            Task(),model(model),particle_id(particle_id),
            other_start(other_start),other_finish(other_finish){
    }
    void exec() override{
        for (int other_id=other_start;other_id<other_finish;++other_id){
            TimeQueueObj tqo = model.predict_collide_of(
                    model.particles[particle_id], model.particles[other_id]);
            result.push(tqo);
        }
    }
};

void Model::init_queue(){
    for (int i=0;i<N;++i){
        auto task = CollideTask(*this, i, i+1,N);
        task.exec();
        while (!task.result.empty()){
            TimeQueueObj& tqo = (task.result.front());
            time_queue.push(tqo);
            ++tqo_counter;
            task.result.pop();
        }
    }
}

void Model::init_queue_parallel(){
    for (int i=0;i<N;++i){
        pool.add_task(
                new CollideTask(*this, i, i+1,N));
    }
    while(pool.finished_task()){
        CollideTask& task = *(CollideTask*)pool.ready_task;
        while (!task.result.empty()){
            TimeQueueObj& tqo = (task.result.front());
            time_queue.push(tqo);
            ++tqo_counter;
            task.result.pop();
        }
    }
}
