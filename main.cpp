
#include "box.h"
#include <chrono>
#include "model.h"
#include "eps_close.h"
#include <io.h>

const string project_dir ("D:\\Ashot\\Desktop\\collisionwise-simulation");

const Real radius_small = 0.01;
const eg::Vector3d box_sides(1,1,1);
const int N_particles = 100;



void time_interval_simulate(){
    Model model(Box(1,1,1));

    model.add_particle(Particle(model.box,{0+0.2,0.5,0.5},
                                {0.1,0,0}, 0.2, 300));
    model.add_particle(Particle(model.box, {1-0.2,0.5,0.5},
                                  {-2,0,0}, 0.05, 1));

    model.dump_inits(project_dir + "\\python");
    model.init_queue();
//    cout<<model.time<<" "<<model.box.time<<endl;
//    for (int q=0;q<5;q++){
//        auto x = model.time_queue.top();model.time_queue.pop();
//        cout<<x<<" validity: "<<model.check_tqo_validity(x)<<
//            " -> "<<model.predict_collide_of(x)<<endl;
//    }


    int bum = 0;
    Real dt = 0.05;
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
            model.dump(project_dir + "\\python\\simulation",
                    "",true);
        }
    }
    cout<<"model hash: "<<model.hash()<<endl;

}


void run(){
    auto start_time = std::chrono::steady_clock::now();

    Model model(Box(box_sides), N_particles, radius_small);

    model.init_queue();
    model.dump_inits(project_dir+"\\python");
    model.dump(project_dir+"\\python\\simulation","", false);
    cout<<"queue length: "<<model.time_queue.size()<<endl;

    model._button  = 0;
    int bum = 0;
    for(int i=0;bum<1000000;i++){
        if(model.time_queue.empty()){
            cout<<"empty time queue"<<endl;
            break;
        }
        int step_result = model.step();
        bum += (2==step_result);
        if (step_result == 2 && bum%10 == 0){
            model.dump(project_dir+"\\python\\simulation","", false);
        }
        if (step_result == 2 && bum%1000 == 0){
            cout<<"bum print"<<endl;
            model.print_stats();
        }

    }
    cout<<"steps finished"<<endl;
    model.print_stats();

    cout<<"model hash: "<<model.hash()<<endl;



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

int main(){

//    time_interval_simulate();
    for(int i=0;i<1;i++){
        run();
    }
    return 0;
}