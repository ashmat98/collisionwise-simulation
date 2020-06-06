//
// Created by Ashot on 5/22/2020.
//

#ifndef COLLISIONS_HASHING_H
#define COLLISIONS_HASHING_H

#include <Eigen/Dense>

struct Hash101{
    long long h = 0;
    long long p = 101;

    void update(long long x) {
        h = h * 101 + x;
    }
    void update(double x){
        update(*((long long *)&x));
    }
    void update(Eigen::Vector3d &v){
        update(v.x());
        update(v.y());
        update(v.z());
    }
    long long hash(){
        return h;
    }
};
#endif //COLLISIONS_HASHING_H
