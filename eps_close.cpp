#include <assert.h>
#include "eps_close.h"
#include <cmath>


tuple<int,int> BasicEngine::find_mn_basically(Real a, Real b, Real r, Real eps){
    /***
     * find integer solution to
     *  a + b * m ~ r * n  eps close
     * 
     ***/
    assert(a < r);
    assert(b < r);
    assert(eps < r);

    for (int m=0;m<=max_m;++m){
        Real n = (a + b * m)/r;
        if (r*abs(round(n) - n) < eps){
            return {m, round(n)};
        }
    }
    return {max_m,-1};
}

vector<tuple<int,int>> find_mns(Real a, Real b, Real r, Real eps){
    /***
     * find integer solution to
     *  a + b * m ~ r * n  eps close
     * 
     ***/
    assert(a < r);
    assert(b < r);
    assert(eps < r);
    vector<tuple<int,int>> V;
    for (int m=0;m<100000;++m){
        Real n = (a + b * m)/r;
        if (r*abs(round(n) - n) < eps){
            V.push_back({m, round(n)});
        }
        if (V.size() == 8){
            break;
        }
    }
    return V;
}

tuple<int,Real,Real> find_overlap(Real a, Real b, Real r, Real eps){
    int m = ((r - eps - a) / b);
    Real p1 = a+b*m;
    Real p2 = a+b*m + b;
    return {m, p1,p2};
}

inline tuple<int,int> get_solution(int m, Real a, Real b, Real r){
    return {m, int(round((a+b*m)/r))};
}

bool is_solution(int m, Real a, Real b, Real r, Real eps){
    Real n = (a + b*m)/r;
    return r*abs(n - round(n)) < eps;
}

tuple<int,int> FastEngine::find_mn_recursive(Real a, Real b, Real r, Real eps){
    /***
     * find integer solution to
     *  a + b * m ~ r * n  eps close
     * 
     ***/

//    cout<<"find mn for: a="<<a<<" b="<<b<<" r="<<r<<" eps="<<eps<<endl;

    if (is_solution(0,a,b,r,eps))
        return get_solution(0,a,b,r);
    
    // assert(0<b && b<r);
    // assert(0<=a && a<=r);
    auto [m1, p1, p2] = find_overlap(a,b,r,eps);
    if (is_solution(m1,a,b,r,eps))
        return get_solution(m1,a,b,r);
    if (is_solution(m1+1,a,b,r,eps))
        return get_solution(m1+1,a,b,r);
    // assert(p2 > r+eps && p1<r-eps);
    
    auto [m2, q1,q2] = find_overlap(p2-r, b,r,eps);
    if (is_solution(m1+1+m2, a,b,r,eps))
        return get_solution(m1+1+m2, a,b,r);
    if (is_solution(m1+1+m2+1, a,b,r,eps))
        return get_solution(m1+1+m2+1, a,b,r);
    // assert(q2 > r+eps && q1<r-eps);
    
    Real q = q1;
    if (q < p1){
        q = q2;
        m2 += 1;
    }

    Real r_new = p2-p1;
    int m;
    if (q-p1 < p2-q){
        auto[m3, n3] = find_mn_recursive(r_new - (r - p1), q - p1, r_new, eps);
        m = m1 + (m2+1) * m3 - (n3-1);
    }
    else{
        auto[m3, n3] = find_mn_recursive(r_new - (p2 - r), p2 - q, r_new, eps);
        m = m1 +1 + (m2) * m3 + (n3-1);
    }
    return get_solution(m,a,b,r);
}


AbstractEngine::AbstractEngine(Real a, Real b, Real eps,int max_m):
        a(a), b(b), eps(eps), current_a(a), max_m(max_m){
    if (isnan(a) || isnan(b) || isnan(eps)){
        exit(687);
    }
};

tuple<int,int> AbstractEngine::next(){
    if (last_m >= max_m){
        return {max_m, -1};
    }
    auto [m,n] = find_mn(current_a,b,1,eps);
    current_a = (current_a + b*(m+1));
    current_a = current_a - floor(current_a);
    last_m += m;
    last_n = (n!=-1)?last_n+n : -1;

    return {last_m++,last_n};
}

tuple<int,int> FastEngine::find_mn(Real a, Real b, Real r, Real eps){
    return find_mn_recursive(a, b, r, eps);
}
tuple<int,int> BasicEngine::find_mn(Real a, Real b, Real r, Real eps){
    return find_mn_basically(a,b,r,eps);
}

// int main(){
//     auto V = find_mns(0.3418, 0.7888,1,0.01);
//     FastEngine eng(0.3418, 0.7888,0.01);

//     for (auto [a,b] :V){
//         auto [e1,e2] = eng.next();
//         printf("%d-%d %d-%d\n", a,e1, b,e2);
//     }
//     return 0;
// }