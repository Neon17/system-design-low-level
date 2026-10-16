#include <iostream>
#include <vector>
using namespace std;

#include "zoo.h"
#include "animalfactory.h"

int main(){
    Zoo zoo;

    zoo.addAnimal(AnimalFactory::create("Lion",  "Simba"));
    zoo.addAnimal(AnimalFactory::create("Eagle", "Bald Eagle"));
    zoo.addAnimal(AnimalFactory::create("Snake", "Cobra"));
    zoo.addAnimal(AnimalFactory::create("Shark", "Great White"));

    zoo.displayAnimals();

    return 0;
}


