//
// Created by jintian on 7/12/17.
//

// provide multi random value

#include <cstdlib>
#include <stdexcept>

#include "time.h"
#include "include/random.h"

using namespace std;

int thor::random::randint(int a, int b){

    if (a >= b){
        throw std::invalid_argument("randint argument a must less than b.");
    }
    srand((unsigned) time(NULL));
    return a + rand() % (a + b -1);
}

float thor::random::randnorm(){
    // return a float value random in 0-1
    return rand();
}


void thor::random::kick_it_random() {
    srand((unsigned int) time(0));
}