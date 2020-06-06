#ifndef MYRANDOM_H
#define MYRANDOM_H

#include "default.h"
#include <random>

extern int seed;
Real randn(Real m=0, Real s=1);
Real randu(Real a=0, Real b=1);
int randi(int a, int b);

#endif