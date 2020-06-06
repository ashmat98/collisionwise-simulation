#ifndef BOX_H
#define BOX_H

#include "default.h"

struct Box{
    eg::Vector3d sides;
    Real time;
    // Box():time(0){};
    Box(double a, double b, double c, 
        Real time=0):time(time){sides << a,b,c;}
    Box(eg::Vector3d sides, Real time=0):sides(sides),time(time){}

    eg::Vector3d lattice_point(eg::Vector3i lp){
        return sides.cwiseProduct(lp.cast<double>());
    }
    eg::Vector3d lattice_point(int m,int n, int k){
        return lattice_point(eg::Vector3i(m,n,k));
    }

    eg::Vector3d closest_point(eg::Vector3d point){
        return point.cwiseQuotient(sides).array().round()
            .matrix().cwiseProduct(sides);
    }

    eg::Vector3d min_difference(eg::Vector3d from, eg::Vector3d to){
        auto p = to - from;
        return p - closest_point(p);
    }

};

#endif