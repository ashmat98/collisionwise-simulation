
#include "box.h"
#include <chrono>
#include "model.h"
#include <io.h>

const string project_dir ("D:\\Ashot\\Desktop\\collisions");

const Real radius = 0.01;
const eg::Vector3d box_sides(1,1,1);
const int N_particles = 100;



void time_interval_simulate(){
    Model model(4, 0.2,Box(1,1,1));
    model.init_queue();

    int bum = 0;
    Real dt = 0.05;
    mkdir((project_dir+"\\python\\simulation").c_str());
    for(int i=0;bum<2;i++){
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
            bum += (2==model.step());

        }
//        model.print_stats();

//        bum += (2==model.step());
//        if (2==model.step()){
//            cout<<"bum print"<<endl;
//            model.print_stats();
//            bum+=1;
//        }
        model.dump(project_dir+"\\python\\simulation");
    }
}


void run(){
    auto start_time = std::chrono::steady_clock::now();

    Model model(N_particles, radius,Box(box_sides));
    model.init_queue();
    model._button  = 0;
    int bum = 0;
    for(int i=0;bum<30;i++){
//        cout<<model.step_counter<<" "<<model.particles[35]<<endl;
        if(model.time_queue.empty()){
            cout<<"empty time queue"<<endl;
            break;
        }
//        cout<<"step "<<model.step()<<endl;
        bum += (2==model.step());
        if (bum%1000 == 0){
            cout<<"bum print"<<endl;
            model.print_stats();
            bum+=1;
        }
//        if (i%10000==0){
//            model.print_stats();
//        }
//        model.print_stats();

//        if (model.step_counter>=1000000){
//            break;
//        }
    }
    model.dump(project_dir+"\\python");
    cout<<model.hash()<<endl;
    cout<<model.particles[54]<<endl;
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
    Particle p1({ 0.57168, 0.624235, 0.236199},
            { 0.88508,  1.14863, -1.15345},radius,box);

    Particle p2({0.687697, 0.681038, 0.618782},
                { 0.940848,  0.698494, -0.775214},radius, box);
    Real collision_time;
    bool collides = collide_with_lattice(
            p2.realtive_to(p1),collision_time, &p1, &p2);
    cout<<collides<<" "<<collision_time<<endl;

    Particle p1_ = p1;
    Particle p2_ = p2;
    p1_.r = p1.at_time(collision_time);
    p2_.r = p2.at_time(collision_time);
    p1_.back_to_box();
    p2_.back_to_box();
    Real f = (p1_.r - p2_.r).norm()/(p1.radius+p2.radius);
    Real f1 = box.min_difference(p1_.r,p2_.r).norm()/(p1.radius+p2.radius);
    cout<<"f"<<f<<" f1"<<f1<<" p1:" << p1_<<"  p2:" <<p2_<< endl;
}
int main(){
    time_interval_simulate();

//    test_collision();
//    return 0;


//    cout<<project_dir<<endl;
//    for(int i=0;i<1;i++){
//        run();
//    }
    return 0;
}