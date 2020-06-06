//
// Created by Ashot on 5/11/2020.
//

#include "time_queue.h"

ostream& operator<<(ostream& o, const TimeQueueObj& tqo){
    o << "time " << tqo.time << "(" << tqo.collides << ") "
      << tqo.p1->id << " with " << tqo.p2->id << " created at " << tqo.obj_creation_time;
    return o;
}

TimeQueueObj::TimeQueueObj(Real obj_creation_time,Real time,
        bool collides,
        Particle &p1, Particle &p2):
        p1(&p1),p2(&p2),time(time),collides(collides),
        obj_creation_time(obj_creation_time) {

}

bool operator<(const TimeQueueObj& a,const TimeQueueObj& b){
    return a.time > b.time;
}
