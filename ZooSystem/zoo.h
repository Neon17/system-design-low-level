#pragma once

#include "animal.h"
#include <vector>
#include <algorithm>

class Zoo {
    vector<Animal*> animals;
    int nextId = 1;

public:
    Zoo() = default;

    void addAnimal(Animal *animal){
        animal->setId(nextId++);
        animals.push_back(animal);
    }

    void displayAnimals(){
        for (auto animal: animals){
            animal->display();
            std::cout<<std::endl;
        }
    }

    void removeAnimal(int id){
        auto it = std::find_if(animals.begin(), animals.end(), [id](Animal* a) { return a->getId() == id; });
        if (it != animals.end()){
            delete *it;
            animals.erase(it);
        }
    }    

    ~Zoo(){
        for (auto animal: animals){
            delete animal;
        }
    }
};
