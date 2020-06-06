//
// Created by Ashot on 5/11/2020.
//

#ifndef COLLISIONS_TIME_QUEUE_H
#define COLLISIONS_TIME_QUEUE_H
#include "particle.h"
#include <queue>

struct TimeQueueObj{
    Particle* p1;
    Particle* p2;
    Real time;
    bool collides;
    Real obj_creation_time;

    TimeQueueObj(Real obj_creation_time, Real time,
                 bool collides,
                 Particle& p1, Particle& p2);
};
ostream& operator<<(ostream& o, const TimeQueueObj& tqo);

bool operator<(const TimeQueueObj& a,const TimeQueueObj& b);

#endif //COLLISIONS_TIME_QUEUE_H
