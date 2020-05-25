//
// Created by selman on 17.10.2019.
//

#ifndef HW2_SELMANRND_H
#define HW2_SELMANRND_H
#include <random>
#include <iostream>
#include <cstdlib>
//#define lut

//#ifdef pseudo
#include <math.h>
#define pseudoA 16843009
#define pseudoC 826366247
//#endif

enum rndtype{REAL,PSEUDO,LUT};

class rndgen{
private:
    std::random_device rd;
    inline unsigned int generate() {
        seed = seed * pseudoA + pseudoC;
        return seed;
    }
    unsigned int seed;
    int ctr=0;
    const double mem[24]={  //look-up table for random
            0.497, 0.380, 0.862, 0.020, 0.391, 0.975, 0.480, 0.905,
            0.759, 0.560, 0.593, 0.744, 0.069, 0.370, 0.790, 0.176,
            0.020, 0.714, 0.539, 0.928, 0.860, 0.717, 0.861, 0.563};
    rndtype mode;
public:
    rndgen(rndtype _mode){
        mode=_mode;
    }
    double random(){
        if (mode==REAL){
            register long long unsigned int temp = ((long long unsigned int) rd() << 32) | (long long unsigned int) rd();
            return temp / ((double) UINT64_MAX);
        }if(mode==PSEUDO){
            return generate() / ((double) UINT32_MAX);
        }if(mode==LUT){
            if(ctr>23){
                std::cerr<<"Look-up table exhausted, try PRG or HRG"<<std::endl;
                exit(-1);
            }
            return mem[ctr++];
        }
    }
};

/*

class rndgen_real {
//#ifdef real
private:
    std::random_device rd;
public:
    double random() {
        register long long unsigned int temp = ((long long unsigned int) rd() << 32) | (long long unsigned int) rd();
        return temp / ((double) UINT64_MAX);
    }
//#endif
};

//#ifdef pseudo
class randgen_pseudo {
public:
    rndgen() {
        std::random_device rd;
        seed = rd();
    }

    rndgen(unsigned int _seed) { seed = _seed; }

    double random() {
        return generate() / ((double) UINT32_MAX);
    }

private:
    inline unsigned int generate() {
        seed = seed * pseudoA + pseudoC;
        return seed;
    }

    unsigned int seed;
//#endif
};

//#ifdef lut
class randgen_lut{
public:
    double random(){
        if(ctr<23){
            std::cerr<<"Look-up table exhausted, try PRG or HRG"<<std::endl;
            exit(-1);
        }
        return mem[ctr++];
    }

private:
    int ctr=0;
   const double mem[24]={
            0.497,
            0.380,
            0.862,
            0.020,
            0.391,
            0.975,
            0.480,
            0.905,
            0.759,
            0.560,
            0.593,
            0.744,
            0.069,
            0.370,
            0.790,
            0.176,
            0.020,
            0.714,
            0.539,
            0.928,
            0.860,
            0.717,
            0.861,
            0.563};
#endif
};
*/

#endif //HW2_SELMANRND_H
