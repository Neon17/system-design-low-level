#pragma once
#include <string>
#include <stdexcept>
#include "mammal.h"
#include "bird.h"
#include "reptile.h"
#include "fish.h"


class AnimalFactory {
    public:
        static Animal* create(const std::string& species, const std::string& name) {
            if (species == "Lion" || species == "Tiger" || species == "Bear") {
                return new Mammal(new Walkable(4), new NonFlyable(), new NonSwimmable(),
                                species, name);
            }
            if (species == "Eagle" || species == "Sparrow") {
                return new Bird(new Walkable(2), new Flyable(2), new NonSwimmable(),
                                species, name);
            }
            if (species == "Snake" || species == "Crocodile") {
                return new Reptile(new NonWalkable(), new NonFlyable(), new Swimmable(0),
                                species, name);
            }
            if (species == "Shark" || species == "Salmon") {
                return new Fish(new NonWalkable(), new NonFlyable(), new Swimmable(2),
                                species, name);
            }
            throw std::invalid_argument("Unknown species: " + species);
        }
};

