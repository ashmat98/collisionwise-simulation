#include "default.h"
#include "particle.h"


Particle::Particle(eg::Vector3d r, 
    eg::Vector3d v, Real radius, Box & box,int id):
    r(r),v(v),radius(radius),box(box),id(id){

}
Particle::Particle(Real radius, Box & box,int id):
    radius(radius),box(box),id(id){
        r << randu(),randu(),randu();
        r = r.cwiseProduct(box.sides);
        v << randn(),randn(),randn();
}

Particle Particle::realtive_to(Particle p2){
    return Particle(r-p2.r, v-p2.v,p2.radius + radius, box);
}

Particle& Particle::back_to_box(){
    r = r - (box.sides.array() * floor(
        r.cwiseQuotient(box.sides).array())).matrix();
        return (*this);
}

bool Particle::does_collide_with(eg::Vector3d point){
    auto R = r - point;
    Real dot = R.dot(v);
    return dot*dot - (R.squaredNorm() - radius*radius) * v.squaredNorm() >= 0; 
}
Real Particle::collision_time(eg::Vector3d point){
    auto R = r - point;
    Real dot = R.dot(v);
    Real D = dot*dot - (R.squaredNorm() - radius*radius) * v.squaredNorm(); 
    if (D<0) return -10000;
    Real t = (-dot - sqrt(D))/v.squaredNorm();

    return t;
    // auto R = point - r;
    // Real dot = R.dot(v);
    // Real h_sqr = R.squaredNorm() - dot*dot/v.squaredNorm();
    // Real t = dot/v.squaredNorm() - sqrt((radius*radius - h_sqr)/v.squaredNorm());
    // return t;
}
eg::Vector3d Particle::at_time(Real t){
    return r + v.cwiseProduct(eg::Vector3d::Constant(t));
}
eg::Vector3d Particle::get_projected_lengths(){
    auto v1 = v.array();
    return (radius/sqrt(1-v1*v1/v.squaredNorm())).matrix(); 
}
Particle Particle::abs(){
        auto va = v.array();
        auto ra = r.array();

        auto r1 = ra* (va>0).cast<double>() - 
            ra * (va<0).cast<double>();
        auto v1 = va.abs();
        return Particle(r1.matrix(), v1.matrix(), radius, box);
    }

ostream& operator<<(
    ostream& o, const Particle& p)
{
    o << "Particle("<<p.id<<") r:{" << p.r.transpose() << "}\tv:{"
    << p.v.transpose() <<"}"<< endl;
    return o;
}
