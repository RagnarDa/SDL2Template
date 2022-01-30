//
// Created by Christoffer Wärnbring on 2021-06-07.
//

#include "Spaceship.h"

Spaceship::Spaceship(double mass) : mass(mass), Attractor(this, &this->mass), Collidable(this, &this->mass) {
}

void Spaceship::update(double deltatime) {
    Attractor::update(deltatime);
    Object::update(deltatime);
    Collidable::update(deltatime);
}