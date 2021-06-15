//
// Created by Christoffer Wärnbring on 2021-06-06.
//

#include "Planet.h"

Planet::Planet(double mass) : Attractor(this, mass), Collidable(this, mass) {}

void Planet::update(double deltatime) {
    Collidable::update(deltatime);
    Object::update(deltatime);
    Attractor::update(deltatime);
}
