
#include "box.h"
#include <chrono>
#include "model.h"
#include "eps_close.h"
#include <io.h>

const string project_dir ("D:\\Ashot\\Desktop\\collisionwise-simulation");

const Real radius_small = 0.01;
const eg::Vector3d box_sides(1,1,1);
const int N_particles = 3;



void time_interval_simulate(){
    Model model(Box(1,1,1));
    model.add_particle(Particle(model.box,{0+0.2,0.5,0.5},
                                {0.1,0,0}, 0.2, 300));
    model.add_particle(Particle(model.box, {1-0.2,0.5,0.5},
                                  {-2,0,0}, 0.05, 1));

    model.dump_inits(project_dir + "\\python");
    model.init_queue();


    int bum = 0;
    Real dt = 0.01;
    mkdir((project_dir+"\\python\\simulation").c_str());
    for(int i=0;bum<10;i++){
        if(model.time_queue.empty()){
            cout<<"empty time queue"<<endl;
            break;
        }
        if (model.time_queue.top().time > model.time + dt){
            model.time += dt;
            for(auto& p : model.particles) {
                model.update_particle_position(p);
            }
        }
        else{
            bool res = (2==model.step());
            bum += res;
            if (res){
                model.print_stats();
            }
        }
        if (model.time > -1) {
            model.dump(project_dir + "\\python\\simulation","",true);
        }
    }
}


void run(){
    auto start_time = std::chrono::steady_clock::now();

    Model model(Box(box_sides), N_particles, radius_small);
    model.init_queue();
    model.dump_inits(project_dir+"\\python");
    model.dump(project_dir+"\\python\\simulation","", false);
    cout<<"queue length: "<<model.time_queue.size()<<endl;

    for(auto p: model.particles){
        cout<<p;
    }
    model._button  = 1;
    int bum = 0;
    for(int i=0;bum<1000000;i++){
        if(model.time_queue.empty()){
            cout<<"empty time queue"<<endl;
            break;
        }
        int step_result = model.step();
        bum += (2==step_result);
        if (step_result == 2){
            model.dump(project_dir+"\\python\\simulation","", false);
        }
        if (step_result == 2 && bum%100 == 0){
            cout<<"bum print"<<endl;
            model.print_stats();
        }

//        if (model.step_counter>=1000000){
//            break;
//        }
    }
    cout<<"steps finished"<<endl;
    model.print_stats();



    for (int q=0;q<0;q++){
        auto x = model.time_queue.top();model.time_queue.pop();
        cout<<x<<" validity: "<<model.check_tqo_validity(x)<<
        " -> "<<model.predict_collide_of(x)<<endl;
    }


    cout<<"Finished"<<endl;

    auto finish_time = std::chrono::steady_clock::now();
    cout<<"Execution time: "<<(std::chrono::duration<double, std::milli>(finish_time - start_time)
            .count())/1000 <<" seconds"<<endl;
}

void test_collision(){
    Box box(box_sides);
    Particle p1(box, { 0.57168, 0.624235, 0.236199},
            { 0.88508,  1.14863, -1.15345},radius_small,1);

    Particle p2(box, {0.687697, 0.681038, 0.618782},
                { 0.940848,  0.698494, -0.775214},radius_small,1);
    Real collision_time;
    bool collides = collide_with_lattice(
            p2.realtive_to(p1),collision_time, &p1, &p2);
    cout<<collides<<" "<<collision_time<<endl;

    Particle p1_ = p1;
    Particle p2_ = p2;
    p1_.r = p1.at_time(collision_time);
    p2_.r = p2.at_time(collision_time);
    p1_.back_to_box_inplace();
    p2_.back_to_box_inplace();
    Real f = (p1_.r - p2_.r).norm()/(p1.radius+p2.radius);
    Real f1 = box.min_difference(p1_.r,p2_.r).norm()/(p1.radius+p2.radius);
    cout<<"f"<<f<<" f1"<<f1<<" p1:" << p1_<<"  p2:" <<p2_<< endl;
}


int main(){

//    test_engine();
//    time_interval_simulate();

//    test_collision();
//    return 0;


//    cout<<project_dir<<endl;
    for(int i=0;i<1;i++){
        run();
    }
    return 0;
}