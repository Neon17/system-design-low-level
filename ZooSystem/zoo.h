#pragma once

#include "animal.h"
#include "logger.h"
#include <vector>
#include <algorithm>

class Zoo {
    std::vector<Animal*> animals;
    int nextId = 1;

public:
    Zoo() = default;

    void addAnimal(Animal *animal){
        animal->setId(nextId++);
        animals.push_back(animal);
        Logger::instance().log("Added animal with ID: " + std::to_string(animal->getId()));
    }

    void displayAnimals(){
        for (auto animal: animals){
            animal->display();
            std::cout<<std::endl;
        }
        Logger::instance().log("Displayed all animals in the zoo.");
    }

    void removeAnimal(int id){
            auto it = std::find_if(animals.begin(), animals.end(), [id](Animal* a) { return a->getId() == id; });
            if (it == animals.end()) {
                Logger::instance().log("No animal with ID " + std::to_string(id));
                return;
            }
            delete *it;
            animals.erase(it);
            Logger::instance().log("Removed animal with ID " + std::to_string(id));
        }    

    ~Zoo(){
        for (auto animal: animals){
            delete animal;
        }
    }
};
