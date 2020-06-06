#include "myrandom.h"


// int seed = time(0);
int seed = 1589052866;
default_random_engine generator(seed);
normal_distribution<Real> std_normal(0,1);
uniform_real_distribution<Real> std_uniform(0.0,1.0);

Real randn(Real m, Real s){
    return std_normal(generator)*s+m;
}
Real randu(Real a, Real b){
    return std_uniform(generator)*(b-a)+a;
}

int randi(int a, int b){
    return int(randu(a,b));
}