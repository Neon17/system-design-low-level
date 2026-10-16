#pragma once
#include <iostream>

class Swim{
protected:
    int numFins = 0;

public:
    virtual void swim() = 0;
    virtual ~Swim() = default;
};

class Swimmable: public Swim{
    public:
        Swimmable(int fins = 0){
            this->numFins = fins;
        }

        void swim(){
            std::cout<<"can swim, num of fins: "<<numFins<<std::endl;
        }
};

class NonSwimmable: public Swim{
    public:
        void swim(){
            std::cout<<"can't swim"<<std::endl;
        }
};

