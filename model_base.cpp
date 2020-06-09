//
// Created by Ashot on 5/11/2020.
//

#include "model.h"
#include <assert.h>

Model::Model(Box box, int num_particles, Real particle_radius, Real particle_mass):
box(box),N(0),pool(MAX_THREADS),time(box.time){
    particles.reserve(num_particles);
    assert(num_particles==0 || (particle_radius > 0 && particle_mass>0));

    start_time_in_universe = std::chrono::steady_clock::now();

    for (int i=0;i<num_particles;i++){
        add_particle(particle_radius, particle_mass);
    }

}

int Model::add_particle(Real radius,Real mass){
    int id = particles.size();
    particles.emplace_back(box, radius,mass, id);
    ++N;
    return id;
}
int Model::add_particle(Particle particle){
    int id = particles.size();
    particle.id = id;
    particles.push_back(particle);
    ++N;
    return id;
}


void Model::distant_collide(Particle& p1, Particle& p2){
    auto R = box.min_difference(p2.r, p1.r);
    auto v1_perp = p1.v.dot(R)/R.squaredNorm() * R;
    auto v2_perp = p2.v.dot(R)/R.squaredNorm() * R;
    auto v_c = (v1_perp*p1.mass + v2_perp*p2.mass)/(p1.mass+p2.mass);
    p1.v = p1.v - v1_perp + 2*v_c-v1_perp;
    p2.v = p2.v - v2_perp + 2*v_c-v2_perp;

//    p1.v = p1.v - v1_perp + v2_perp;
//    p2.v = p2.v - v2_perp + v1_perp;
    p1.last_collision_time = time;
    p2.last_collision_time = time;
}
void Model::update_particle_position(Particle& p){
    p.r = p.r + p.v * (time - p.last_update_time);
//    p.back_to_box_inplace();
    p.last_update_time = time;
}
void Model::update_particle_collisions_with_others(Particle p) {
    cout<<"model update_particle_collisions_with_others"<<endl;
    cout<<p<<endl;
//    if(_button==1){
//        exit(0);
//    }
    for (auto other : particles){
        if (other.id == p.id){continue;}

        time_queue.push(predict_collide_of(p, other));
        ++tqo_counter;

    }
}


bool Model::check_tqo_validity(const TimeQueueObj &tqo) {
    return (tqo.obj_creation_time >= tqo.p1->last_collision_time
            && tqo.obj_creation_time >= tqo.p2->last_collision_time);
}
inline int _max(int a, int b){
    return (a<b)?b:a;
}
int Model::step() {
    max_queue_size = _max(max_queue_size, time_queue.size());
    if (time_queue.empty()){
        return 0;
    }
    TimeQueueObj tqo = time_queue.top();
    ++step_counter;
    if (_button == 1) {
        cout << "tqo out " << tqo << endl;
    }
    if (!check_tqo_validity(tqo)){
        time_queue.pop();
        return 1;
    }
    time = tqo.time; // increase model time
    if (tqo.time<0){
        cout<<"negative time "<<tqo<<endl;
        print_stats();
        exit(521);
    }
    Particle& p1 = *tqo.p1;
    Particle& p2 = *tqo.p2;


    update_particle_position(p1);
    update_particle_position(p2);
    if (tqo.collides){
        distant_collide(p1, p2);
        update_particle_collisions_with_others(p1);
        update_particle_collisions_with_others(p2);
        ++collision_counter;
        return 2;
    }
    else {
        time_queue.emplace(predict_collide_of(p1, p2));
        tqo_counter++;
        return 3;
    }
}

TimeQueueObj Model::predict_collide_of(Particle& p1, Particle& p2){

    Real collision_time;
    bool collides = collide_with_lattice(
            p2.realtive_to(p1),collision_time, &p1, &p2);
    if (collides){
        Particle p1_ = p1;
        Particle p2_ = p2;
        p1_.r = p1.at_time(collision_time);
        p2_.r = p2.at_time(collision_time);
//        p1_.back_to_box_inplace();
//        p2_.back_to_box_inplace();
        Real f = box.min_difference(p1_.r,p2_.r).norm()/(p1.radius+p2.radius);
        if (abs(f-1)>0.01){
            cout<<"f"<<f<<" p1:" << p1<<"  p2:" <<p2<< endl;
        }

    }
    if (_button == 1) {
        cout << "tqo added " << TimeQueueObj(time, time + collision_time, collides, p1, p2) << endl;
    }
    return TimeQueueObj(time, time+collision_time, collides, p1, p2);
}

Model::~Model() {

}


