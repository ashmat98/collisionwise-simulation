#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include <tuple>
using namespace std;
typedef double real;

int seed = time(0);
// int seed = 1588620100;

default_random_engine generator(seed);
normal_distribution<real> std_normal(0,1);
uniform_real_distribution<real> std_uniform(0.0,1.0);


real randn(real m=0, real s=1){
    return std_normal(generator)*s+m;
}

real randu(real a=0, real b=1){
    return std_uniform(generator)*(b-a)+a;
}




struct Direction
{
	real a,v,x;
    // real a;
    // real x;
	Direction(real a, real v, real x):a(a),v(v),x(x){}
	bool operator <(const Direction & obj) const
	{
		return a/v < obj.a/obj.v;
	}
    tuple<real,real,real> get(){
        return {a,v,x};
    }
};


class Run{
    public:
    real rho = 0.01;

    real a1, a2, a3;
    real v1, v2, v3;
    real x1, x2, x3;
    real rho2, rho3;
    real dt1;

    Run(){
        _init_();
    }

    void _init_(){
        real ax=1, ay=2, az=3;
        real vx = randn();
        real vy = randn();
        real vz = randn();
        
        real x0 = randu(0, ax);
        real y0 = randu(0, ay);
        real z0 = randu(0, az);

        if (vx<0){vx*=-1;x0 = ax-x0;}
        if (vy<0){vy*=-1;y0 = ay-y0;}
        if (vz<0){vz*=-1;z0 = az-z0;}

        vector<Direction> to_sort = {Direction(ax,vx, x0),
            Direction(ay,vy,y0),
            Direction(az,vz,z0)};
        sort(to_sort.begin(), to_sort.end());

        tie(a1,v1,x1) = to_sort[0].get();
        tie(a2,v2,x2) = to_sort[1].get();
        tie(a3,v3,x3) = to_sort[2].get();

        // dt1 = -x1/v1;
        // x1 = x1 + v1 * dt1;
        // x2 = x2 + v2 * dt1;
        // x3 = x3 + v3 * dt1;
            
        rho2 = rho / cos(atan(v2/v1));
        rho3 = rho / cos(atan(v3/v1));
    }
    inline real square(real x){
        return x*x;
    }
    inline real sum_square(real x, real y, real z){
        return x*x+y*y+z*z;
    }
    inline real norm(real x, real y, real z){
        return sqrt(sum_square(x,y,z));
    }

    real check_collision_with(real d1, real d2, real d3){
        real t = (d1-a1-x1)/v1;
        real s1 = d1 - (x1 + t * v1);
        real s2 = d2 - (x2 + t * v2);
        real s3 = d3 - (x3 + t * v3);
        // printf("%0.4f %0.4f %0.4f\n", d1, d2, d3);

        real dot_product = s1*v1 + s2*v2 + s3*v3;
        real delta_t1 = dot_product/sum_square(v1,v2,v3); //TODO: sdf

        real h_square = sum_square(s1,s2,s3) - square(dot_product)/sum_square(v1,v2,v3);

        if ( h_square < square(rho) ){
            real delta_t2 = - (square(rho) - h_square) / norm(v1,v2,v3);
            return t + delta_t1 + delta_t2; 
        }
        return -1;
    }
    
    real find_collision(){
        real t_collid = -1;
        int n;
        for (n=1;n<100;++n){
            real t = (n*a1 - x1)/v1;
            real d1 = round((x1 + t * v1)/a1)*a1;
            // printf("aaa %0.4f\n", d1);

            real d2 = round((x2 + t * v2)/a2)*a2;
            real d3 = round((x3 + t * v3)/a3)*a3;

            t_collid = check_collision_with(d1,d2,d3);
            // printf(".");
            if (t_collid > 0){
                break;
            }
        }
        return t_collid;
        // printf("%d\t%0.4f\n",n, t_collid);
    }

    inline bool eps_close(real a, real b, real eps){
        return abs(a-b)<eps;
    }

    tuple<int, int, int> n_range_2(int m){
        for (int i=0;i<100;i++){
            m+=1;
            int lower = ceil((m*a2 - rho2 - x2)/(a1*(v2/v1)));
            int upper = floor((m*a2 + rho2 - x2)/(a1*(v2/v1)));
            if (lower <= upper)
                return {lower, upper, m};
        }
        return {-1,-1, m};
    }
    
    tuple<int, int> n_range_3(int k){
        real lower = (k*a3 - rho3 - x3)/(a1*(v3/v1));
        real upper = (k*a3 + rho3 - x3)/(a1*(v3/v1));
        return {ceil(lower), floor(upper)};
    }

    void print(){
        printf("v: %0.3f, %0.3f %0.3f\n", v1, v2,v3);
        printf("x: %0.3f, %0.3f %0.3f\n", x1, x2, x3);
        printf("a: %0.3f, %0.3f %0.3f\n", a1, a2, a3);
        printf("p: %0.5f %0.5f\n", rho2, rho3);
    }
};

int main(){
    printf("seed: %d\n\n", seed);
    int T=1000*1000;
    int N = 0;
    for(int t=0;t<T;t++){
        auto r = Run();
        // r.print();

        N+= 0<r.find_collision();
    }
    printf("%d  %0.3f %%\n", N, N*100.0/T);
    return 0;
}