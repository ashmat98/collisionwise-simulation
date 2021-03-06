//
// Created by Ashot on 6/19/2020.
//

#ifndef COLLISIONS_MAIN_TIME_INTERVAL_H
#define COLLISIONS_MAIN_TIME_INTERVAL_H
#include "default.h"
#include "box.h"
#include "model.h"

void time_interval_simulate(){
    Model model(Box(1,1,1), 10,0.06, 1);
//    model.add_particle(Particle(model.box,{0+0.2,0.5,0.5},
//                                {0.1,0,0}, 0.2, 300));
//    model.add_particle(Particle(model.box, {1-0.2,0.5,0.5},
//                                {-2,0,0}, 0.05, 1));
    model.make_2D();
    model.create_dumpfile();
    model.init_queue();
    model.dump();
//    cout<<model.time<<" "<<model.box.time<<endl;
//    for (int q=0;q<5;q++){
//        auto x = model.time_queue.top();model.time_queue.pop();
//        cout<<x<<" validity: "<<model.check_tqo_validity(x)<<
//            " -> "<<model.predict_collide_of(x)<<endl;
//    }


    int bum = 0;
    Real dt = 0.01;
    Real last_dump = 0;
    for(int i=0;bum<400;i++){
        if(model.time_queue.empty()){
            cout<<"empty time queue"<<endl;
            break;
        }
        if (model.time_queue.top().time > last_dump + dt){
            model.time = last_dump + dt;
            for(auto& p : model.particles) {
                model.update_particle_position(p);
            }
            model.dump(true);
            last_dump = model.time;
        }
        else{
            bool res = (2==model.step());
            bum += res;
            if (res){
                model.print_stats();
            }
        }
    }
    cout<<"model hash: "<<model.hash()<<endl;

}
#endif //COLLISIONS_MAIN_TIME_INTERVAL_H
