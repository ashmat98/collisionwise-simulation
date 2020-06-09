//
// Created by Ashot on 5/22/2020.
//

#include "hashing.h"

void Hash101::update(long long x) {
    h = h * 101 + x;
}

void Hash101::update(double x){
    update(*((long long *)&x));
}

void Hash101::update(Eigen::Vector3d &v){
    update(v.x());
    update(v.y());
    update(v.z());
}
long long Hash101::hash(){
    return h;
}
