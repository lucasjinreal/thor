//
// Created by jintian on 18-1-11.
//

#ifndef CAO_DOG_H
#define CAO_DOG_H

#include <iostream>
#include <string>
#include <vector>

#include "thor/logging.h"


using namespace std;

class Dog{
public:

    Dog(string name);
    ~Dog();


    vector<string> getAbilities();
    string getName();

private:
    string _name;

};

#endif //CAO_DOG_H
