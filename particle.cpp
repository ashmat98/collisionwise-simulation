#include "default.h"
#include "particle.h"

#include <utility>


Particle::Particle(Box & box,eg::Vector3d r,
    eg::Vector3d v, Real radius, Real mass, int id):
    r(std::move(r)),v(std::move(v)),radius(radius),mass(mass),box(box),id(id){

}
Particle::Particle(Box & box, Real radius, Real mass, int id):
    radius(radius),mass(mass),box(box),id(id){
        r << randu(),randu(),randu();
        r = r.cwiseProduct(box.sides);
        v << randu(),randu(),randu();
}

Particle Particle::realtive_to(Particle p2){
    return Particle(box, r-p2.r, v-p2.v,p2.radius + radius,
            mass * p2.mass / (mass * p2.mass));
}

Particle& Particle::back_to_box_inplace(){
    r = r - (box.sides.array() * floor(
        r.cwiseQuotient(box.sides).array())).matrix();
        return (*this);
}

eg::Vector3d Particle::back_to_box(){
    return r - (box.sides.array() * floor(
            r.cwiseQuotient(box.sides).array())).matrix();
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
    /***
     * Shadow of the ball on the lattice lines for each direction
     * Pr = a/cos(b)  sin(b) = vi/v
     */
    auto v1 = v.array();
    return (radius/sqrt(EPS+1-v1*v1/v.squaredNorm())).matrix();
}
Particle Particle::abs(){
        auto va = v.array();
        auto ra = r.array();

        auto r1 = ra* (va>0).cast<double>() - 
            ra * (va<0).cast<double>();
        auto v1 = va.abs();
        return Particle(box, r1.matrix(), v1.matrix(), radius,1);
    }

ostream& operator<<(
    ostream& o, const Particle& p)
{
    o << "Particle("<<p.id<<") r:{" << p.r.transpose() << "}\tv:{"
    << p.v.transpose() <<"}"<< endl;
    return o;
}
