//
// Created by Ashot on 6/19/2020.
//

#ifndef COLLISIONS_MAIN_SIMULATION_H
#define COLLISIONS_MAIN_SIMULATION_H
#include "default.h"
#include "box.h"
#include "model.h"
#include <chrono>
#include "eps_close.h"

const Real radius_small = 0.05;
const eg::Vector3d box_sides(1,1,1);
//const int N_particles = 100;

void run_simulation(int N_particles){
    auto start_time = std::chrono::steady_clock::now();

    Model model(Box(box_sides), N_particles, radius_small);

    model.init_queue();
    model.create_dumpfile();

    cout<<"queue length: "<<model.time_queue.size()<<endl;

    model._button  = 0;
    for(int i=0;model.collision_counter<10000;i++){
        if(model.time_queue.empty()){
            cout<<"empty time queue"<<endl;
            break;
        }
        int step_result = model.step();

        if (step_result == 2 && model.collision_counter%10 == 0){
            model.dump(false);
        }
        if (step_result == 2 && model.collision_counter%1000 == 0){
            cout<<"bum print"<<endl;
            model.print_stats();
        }
        if (model.step_counter> 2455526)
            printf("step: %d %d\n",model.step_counter, step_result);
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
#endif //COLLISIONS_MAIN_SIMULATION_H
