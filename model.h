//
// Created by Ashot on 5/11/2020.
//

#ifndef COLLISIONS_MODEL_H
#define COLLISIONS_MODEL_H
#include "default.h"
#include "collide_two_particles.h"
#include "time_queue.h"
#include <queue>
#include "thread_pool.h"
#include "hashing.h"

struct Model{

    vector<Particle> particles;
    priority_queue<TimeQueueObj> time_queue;

    Box & box;
    int N;
    ThreadPool pool;
    Real& time;

    /// stats
    int collision_counter = 0;
    int tqo_counter = 0;
    int step_counter = 0;
    int max_queue_size = 0;
    ///
    int _button=0;

    Model(int num_particles, Real particle_radius, Box box);
    int add_particle();
    void init_queue();
    void init_queue_parallel();

    void distant_collide(Particle& p1, Particle& p2);
    void update_particle_position(Particle &p);
    void update_particle_collisions_with_others(Particle& p);
    TimeQueueObj predict_collide_of(Particle& p1, Particle& p2);
    TimeQueueObj predict_collide_of(TimeQueueObj& old_tqo){
        return predict_collide_of(*old_tqo.p1, *old_tqo.p2);
    }
    int step();
    bool check_tqo_validity(const TimeQueueObj& tqo);

    void print_stats(){
        int particles_in_box = 0;
        Real mean_v = 0;

        for(auto& p:particles){
            particles_in_box+=p.in_box();
            mean_v += p.v.norm()/particles.size();
        }

        cout<<"STEP.............."<<step_counter<<endl;
        cout<<"TIME.............."<<time<<endl;
        cout<<"Collisions........"<<collision_counter<<endl;
        cout<<"tqos.............."<<tqo_counter<<endl;
        cout<<"queue len........."<<time_queue.size()<<endl;
        cout<<"max queue len....."<<max_queue_size<<endl;
        cout<<"p in box.........."<<particles_in_box<<"/"<<particles.size()<<endl;
        cout<<"mean velocity....."<<mean_v<<endl;

        cout<<endl;
    }


    long long hash(){
        Hash101 h;
        h.update(box.sides);
        h.update(time);
        for (auto& p : particles){
            h.update(p.r);
            h.update(p.v);
        }
        return h.hash();
    }

    void dump(string path, string name="");

    ~Model();

};
#endif //COLLISIONS_MODEL_H
