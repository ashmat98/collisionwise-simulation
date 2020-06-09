//
// Created by Ashot on 5/22/2020.
//

#ifndef COLLISIONS_HASHING_H
#define COLLISIONS_HASHING_H

#include <Eigen/Dense>

struct Hash101{
    long long h = 0;
    long long p = 101;

    void update(long long x);
    void update(double x);
    void update(Eigen::Vector3d &v);
    long long hash();
};
#endif //COLLISIONS_HASHING_H
