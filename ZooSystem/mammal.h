#pragma once
#include "animal.h"

class Mammal: public Animal {
    public:
        Mammal(Walk *walk, Fly *fly, Swim *swim, std::string species = "", std::string name = ""): Animal(walk, fly, swim, species, name) {
            
        }

        void display() {
            std::cout<<"Mammal, Name = "<<name<<", Species = "<<species<<std::endl;
            walk->walk();
            fly->fly();
        }
};