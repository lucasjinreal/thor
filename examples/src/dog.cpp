//
// Created by jintian on 18-1-11.
//


#include "../include/dog.h"
#include "thor/os.h"


Dog::Dog(string name) {
    thor::Log(kInfo, "Building your dog...");
    this->_name = name;
}

Dog::~Dog() {

}

vector<string> Dog::getAbilities() {
    vector<string> abilities;
    abilities.push_back("Eat");
    abilities.push_back("WelcomeHome");
    abilities.push_back("Sleep");
    abilities.push_back("Running");
    return abilities;
}

string Dog::getName() {
    return _name;
}
