#include "default.h"

// tuple<int,int> find_mn_recursive(real a, real b, real r, real eps);

struct AbstractEngine{
    /***
     * find integer solution to
     *  a + b * m ~ n  eps close
     * 
     ***/
    Real a;
    Real b;
    Real eps;
    Real current_a;
    int last_m=0;
    int last_n=0;
    int max_m=INF;

    AbstractEngine(Real a, Real b, Real eps, int max_m=INF);
    virtual tuple<int,int> find_mn(Real a, Real b, Real r, Real eps) =0;
    tuple<int,int> next();
};

struct FastEngine : AbstractEngine{
    using AbstractEngine::AbstractEngine;



    tuple<int,int> find_mn(Real a, Real b, Real r, Real eps);

private:
    tuple<int,int> find_mn_recursive(Real a, Real b, Real r, Real eps);

};

struct BasicEngine : AbstractEngine{
    using AbstractEngine::AbstractEngine;
    tuple<int,int> find_mn(Real a, Real b, Real r, Real eps);

private:
    tuple<int,int> find_mn_basically(Real a, Real b, Real r, Real eps);

};

bool is_solution(int m, Real a, Real b, Real r, Real eps);