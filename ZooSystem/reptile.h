#pragma once
#include "animal.h"

class Reptile: public Animal {
    public:
        Reptile(Walk *walk, Fly *fly, Swim *swim, std::string species = "", std::string name = ""): Animal(walk, fly, swim, species, name) {

        }

        void display() {
            std::cout<<"Reptile, Name = "<<name<<", Species = "<<species<<std::endl;
            walk->walk();
            fly->fly();
            swim->swim();
        }
};
