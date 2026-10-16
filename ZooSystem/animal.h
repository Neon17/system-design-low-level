#pragma once
#include <iostream>
#include "walk.h"
#include "fly.h"
#include "swim.h"

class Animal {
    protected:
        Walk *walk;
        Fly *fly;
        Swim *swim;

        int id = 0;
        std::string name;
        std::string species;
    public:
        Animal(Walk *walk, Fly *fly, Swim *swim, std::string species = "", std::string name = "") {
            this->walk = walk;
            this->fly = fly;
            this->swim = swim;
            this->name = name;
            this->species = species;
        }

        void setId(int id) {this->id = id;}
        int getId() {return id;}

        void setName(std::string name) {this->name = name;}
        std::string getName() {return name;}

        void setSpecies(std::string species) {this->species = species;}
        std::string getSpecies() {return species;}

        virtual void display() = 0;
        virtual ~Animal() = default;
};


