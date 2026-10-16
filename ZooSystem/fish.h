#pragma once
#include <iostream>
#include "animal.h"

class Fish: public Animal {
public:
    Fish(Walk *walk, Fly *fly, Swim *swim, std::string species = "", std::string name = ""): Animal(walk, fly, swim, species, name) {

    }

    void display() {
        std::cout<<"Fish, Name = "<<name<<", Species = "<<species<<std::endl;
        walk->walk();
        fly->fly();
        swim->swim();
    }
};
