//
// Created by Ashot on 5/11/2020.
//

#include "model.h"
#include <assert.h>



Model::Model(Box box_, int num_particles, Real particle_radius,
        Real particle_mass, Real characteristic_free_run_time):
box(box_),N(0),pool(MAX_THREADS),time(box.time),
characteristic_free_run_time(characteristic_free_run_time){
    particles.reserve(num_particles);
    assert(num_particles==0 || (particle_radius > 0 && particle_mass>0));

    start_time_in_universe = std::chrono::steady_clock::now();

    for (int i=0;i<num_particles;i++){
        add_particle(particle_radius, particle_mass);
    }
}
void Model::make_2D() {
    for (Particle& p : particles){
        p.r(2) = 0;
        p.v(2) = 0;
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

    p1.last_collision_time = time;
    p2.last_collision_time = time;
}
void Model::update_particle_position(Particle& p){
    p.r = p.r + p.v * (time - p.last_update_time);
//    p.back_to_box_inplace();
    p.last_update_time = time;
}
void Model::update_particle_collisions_with_others(Particle p) {
    for (auto other : particles){
        if (other.id == p.id){continue;}
        time_queue.push(predict_collide_of(p, other));
        ++tqo_counter;
    }
}


bool Model::check_tqo_validity(const TimeQueueObj &tqo) {
    return (tqo.obj_creation_time >= particles[tqo.p1].last_collision_time
            && tqo.obj_creation_time >= particles[tqo.p2].last_collision_time);
}
inline int _max(int a, int b){
    return (a<b)?b:a;
}
int Model::step() {
    /**
     * return 1 -- tqo not valid
     * return 2 -- tqo collides
     * return 3 -- tqo not collides
     */
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
    Particle& p1 = particles[tqo.p1];
    Particle& p2 = particles[tqo.p2];

    for (auto& p : particles){
        update_particle_position(p);
    }

//    update_particle_position(p1);
//    update_particle_position(p2);

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

//    if (collision_counter == 4211){
//        cout<<
//    }

    Real collision_time;
    bool collides = collide_with_lattice(
            p2.realtive_to(p1),collision_time, &p1, &p2,
            characteristic_free_run_time);
    if (_button == 1) {
        cout<<"relative particle: "<<p2.realtive_to(p1)<<endl;
        cout<<"   "<<p2.realtive_to(p1).back_to_box().transpose()<<endl;

        cout << "collides: " << collides << " collision time: " << collision_time << endl;
    }
    if (collides){
        Particle p1_ = p1;
        Particle p2_ = p2;
        p1_.r = p1.at_time(collision_time);
        p2_.r = p2.at_time(collision_time);
//        p1_.back_to_box_inplace();
//        p2_.back_to_box_inplace();
        Real f = box.min_difference(p1_.r,p2_.r).norm()/(p1.radius+p2.radius);
        if (_button || abs(f-1)>0.01){
            cout<<collision_counter<<endl;
            cout<<"f"<<f<<" p1:" << p1<<"  p2:" <<p2<< endl;
//            dump(PROJECT_DIR,"ERROR-FILE.bin");
            exit(0);
        }

    }
    if (_button == 1) {
        cout << "tqo added " << TimeQueueObj(time, time + collision_time, collides, p1.id, p2.id) << endl;
    }
    return TimeQueueObj(time, time+collision_time, collides, p1.id, p2.id);
}

Model::~Model() {
    if (model_file.is_open())
        model_file.close();
}


