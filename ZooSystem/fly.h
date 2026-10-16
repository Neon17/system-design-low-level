#pragma once
#include <iostream>
class Fly {
    protected:
        int numOfWings = 2;

    public:
        virtual void fly() = 0;
        virtual ~Fly() = default;
};

class Flyable: public Fly {
    public:
        Flyable(int numOfWings) {
            this->numOfWings = numOfWings;
        }

        void fly(){
            std::cout<<"can fly, num of wings: "<<numOfWings<<std::endl;
        }
};

class NonFlyable: public Fly {
    public:
        void fly(){
            std::cout<<"can't fly"<<std::endl;
        }
};
