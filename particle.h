#ifndef PARTICLE_H
#define PARTICLE_H

#include "default.h"
#include "myrandom.h"
#include "box.h"

struct Particle{
    eg::Vector3d r;
    eg::Vector3d v;
    Real radius;
    Real mass;
    Box & box;

    Real last_update_time = 0;
    Real last_collision_time = -INF;
//    Real
    bool collides=false;

    int id;

    Particle(Box & box, eg::Vector3d r,
    eg::Vector3d v,Real radius,Real mass=1,int id=-1);
    Particle(Box & box, Real radius, Real mass=1, int id=-1);

    Particle realtive_to(Particle p2);
    Particle& back_to_box_inplace();
    eg::Vector3d back_to_box();

    bool in_box(){
        return  (r.array()<box.sides.array()).all() &&(r.array()>0).all();
    }
    bool does_collide_with(eg::Vector3d point);
    Real collision_time(eg::Vector3d point);

    eg::Vector3d at_time(Real t);
    eg::Vector3d get_projected_lengths();
    Particle abs();

};

ostream& operator<<(
    ostream& o, const Particle& a);

// struct RandomParticle : Particle{
//     RandomParticle(Real radius, Box & box)
//     :Particle({
//     }
// }

#endif