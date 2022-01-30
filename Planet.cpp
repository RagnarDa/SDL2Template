//
// Created by Christoffer Wärnbring on 2021-06-06.
//

#include "Planet.h"

Planet::Planet(double mass) : mass(mass), Attractor(this, &this->mass), Collidable(this, &this->mass), Mergeable(this, &this->mass){}

void Planet::update(double deltatime) {
    Mergeable::update(deltatime);
    Collidable::update(deltatime);
    Object::update(deltatime);
    Attractor::update(deltatime);
}
