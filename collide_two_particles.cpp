// #include "myrandom.h"
#include "eps_close.h"
#include "collide_two_particles.h"


bool test_cllision(){
    Real radius = 0.01;
    bool test_result = true;
    Box box(1,2,3);
    Particle p1(box, radius,1);
    Particle p2(box, radius,1);
    auto p3 = p1.realtive_to(p2);
    p3.back_to_box_inplace();

    auto point = p3.r + p3.v * 40+
        eg::Vector3d(0,0,p3.radius/2);
    test_result &= (p3.does_collide_with(point) == true);

    Real t = p3.collision_time(point);
    auto col_point = p3.r + p3.v * t;
    test_result &= abs((point - col_point).norm() - p3.radius) < EPS;

    printf("Collision test: ");
    if (test_result)
        printf("PASSED");
    else
        printf("FAILED");
    return test_result;
}

struct V4{
    double side, r, v, proj;
};
bool comp1(V4& a, V4& b){
    return a.v > b.v;
}

const Real eps = 1e-20;

bool collide_with_lattice(Particle particle, 
    Real& collision_time, Particle* p1, Particle* p2, Real forward_time){

    Particle p = particle.abs();
    p.back_to_box_inplace();


//    cout<<p<<endl;
//    cout<<*p1<<endl;
//    cout<<*p2<<endl;
//    cout<<endl;


    auto proj = p.get_projected_lengths();
    eg::Matrix<double,4,3> M;
    M.row(0) = p.box.sides;
    M.row(1) = p.r;
    M.row(2) = p.v;
    M.row(3) = proj;

    // cout<<M<<endl;
    V4 * q = (V4*)M.data();
    // cout<<q->x<<" "<<q->y<<" "<<q->z<<" "<<q->w<<endl;
    sort(q, q+3, comp1);
    // cout<<M<<endl;
    /////0,1,2,3
    //// a,x,v,ro
    Real delta_t = M(0,0)/M(2,0); //   a1 / v1
    Real t_0 = -M(1,0)/M(2,0);    //  -x1 / v1

    Real b2 = (M(2,1)/(M(2,0))) * (M(0,0)/M(0,1));
    //   b2 = (v2/v1)(a1/a2) = v2/a2 * delta_t
    Real a2 = (M(1,1) - M(1,0)*(M(2,1)/(M(2,0))))/M(0,1);
    //   a2 = (x2 - x1(v2/v1)) / a2
    Real eps2 = M(3,1) / M(0,1);
    //   eps2 = ro2 / a2

    Real b3 = (M(2,2)/(M(2,0))) * (M(0,0)/M(0,2));
    //   b3 = (v3/v1)(a1/a3)
    Real a3 = (M(1,2) - M(1,0)*(M(2,2)/(M(2,0))))/M(0,2);
    //   a3 = (x3 - x1(v3/v1)) / a3
    Real eps3 = M(3,2) / M(0,2);
    //   eps3 = ro3 / a3
    ////
//    Real a,b,eps,  a_other, b_other, eps_other;
    if (eps2 < eps3){ // ro2 < ro3
        swap(a2,a3);
        swap(b2,b3);
        swap(eps2,eps3);
    }

    FastEngine eng(a3, b3, eps3 );

    bool found = false;
    while (true){
        auto [m,n] = eng.next();
        if (n==-1){
            break;
        }
        Real t_next = delta_t * m;
        if (t_next > forward_time){
            collision_time = t_0 + forward_time - abs(randn(0,0.001));
            break;
        }
        Real t = t_0 + t_next;
        if (is_solution(m,a2,b2,1,eps2)){
            auto point_t = p.at_time(t);
            auto point_l = p.box.closest_point(point_t);
            collision_time = p.collision_time(point_l);
            if (collision_time>0+EPS){
                found = true;
                break;
            }
        }
        if (m > MAX_M){
            collision_time = t+randn(0,0.001);
            break;
        }
    }
//    if ((collision_time)<0) {
//        cout << "c("<<found<<")" << collision_time << " p1:" << *p1<<"  p2:" <<*p2<< endl;
//    }
    return found;
}

// int main(){
//     cout<<"seed: "<<seed<<endl;

//     test_cllision();

//     printf("Hello World.\n");
//     Box box(1,2,3);
//     Particle p1(radius, box);
//     Particle p2(radius, box);
//     Real ct;
    
//     bool res = collide_with_lattice(p1.realtive_to(p2), ct);
//     cout<<res<<" "<<ct;
    
       
//     return 0;
// }