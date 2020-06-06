#include <chrono>
#include <ctime>
#include <vector>
#include <iostream>
// #include <vector>
#include <numeric>
// #include <string>
// #include <functional>

#include "eps_close.h"

int seed = 1588767410;

#include <random>
default_random_engine generator(seed);
normal_distribution<real> std_normal(0,1);
uniform_real_distribution<real> std_uniform(0.0,1.0);
real randn(real m=0, real s=1){
    return std_normal(generator)*s+m;
}
real randu(real a=0, real b=1){
    return std_uniform(generator)*(b-a)+a;
}



inline double compute_mean(const vector<double>& numbers){
    return accumulate(numbers.begin(), numbers.end(), 0.0) / numbers.size();
}

double compute_std(const vector<double>& numbers){
    double mean = compute_mean(numbers);
    auto const add_square = [mean](double sum, int i) {
        auto d = i - mean;
        return sum + d*d;
    };
    double total = accumulate(numbers.begin(), numbers.end(), 0.0, add_square);
    return total / (numbers.size() - 1);
}

int main(){
    real eps = 0.001;

    int T = 1000;
    int TT = 1000;
    vector<double> times;
    
    int minus1 = 0;
    for (int t=0;t<T;++t){
        auto simd_start = std::chrono::steady_clock::now();
        for(int tt=0;tt<TT;++tt){
            real a = randu();
            real b = randu();
            auto [m,n] = find_mn_for(a,b,1,eps);
            if (m == -1){
                minus1+=1;
            }
        }
        auto simd_end = std::chrono::steady_clock::now();
        double time = (std::chrono::duration<double, std::milli>(simd_end - simd_start)
                            .count());
        times.push_back(time);

    }
    cout<<"time: "<<compute_mean(times)<<" +- "<<compute_std(times)<<endl;

    return 0;
}