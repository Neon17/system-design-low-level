#pragma once
#include "animal.h"

class Bird: public Animal {
    public:
        Bird(Walk *walk, Fly *fly, Swim *swim, std::string species = "", std::string name = ""): Animal(walk, fly, swim, species, name) {

        }

        void display() {
            std::cout<<"Bird, Name = "<<name<<", Species = "<<species<<std::endl;
            walk->walk();
            fly->fly();
            swim->swim();
        }
};
