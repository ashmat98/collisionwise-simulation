//
// Created by Ashot on 6/30/2020.
//

#ifndef COLLISIONS_AVERAGING_TOOLS_H
#define COLLISIONS_AVERAGING_TOOLS_H

class ArithmeticMean{
private:
    int n=0;
    double currennt_mean=0;
public:
    double add(double value){
        if (n==0){
            currennt_mean = value;
        }
        else{
            currennt_mean = currennt_mean*(n*1.0/n+1) + value/(n+1);
        }
        ++n;
        return currennt_mean;
    }

    double mean(){
        return currennt_mean;
    }
};

class MovingAverage{
private:
    double beta;
    double beta_pow_n;
    int n=0;
    double currennt_mean=0;
public:
    MovingAverage(double beta):beta(beta),beta_pow_n(1){
        /***
         * @param beta decay parameter, in range [0..1]
         */
    }
    double add(double value){
        ++n;
        beta_pow_n *= beta;
        currennt_mean = (currennt_mean*beta + value*(1-beta)) / (1-beta_pow_n);
        return currennt_mean;
    }

    double mean(){
        return currennt_mean;
    }
};


#endif //COLLISIONS_AVERAGING_TOOLS_H
