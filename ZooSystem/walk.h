#pragma once

#include <iostream>

class Walk {
    protected:
        int numOfLegs = 0;

    public:
        virtual void walk() = 0;
        virtual ~Walk() = default;
};

class Walkable: public Walk {
    public:
        Walkable(int numOfLegs) {
            this->numOfLegs = numOfLegs;
        }

        void walk(){
            std::cout<<"can walk, num of legs: "<<numOfLegs<<std::endl;
        }
};

class NonWalkable: public Walk {
    public:
        void walk(){
            std::cout<<"can't walk"<<std::endl;
        }
};