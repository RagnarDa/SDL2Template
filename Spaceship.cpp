//
// Created by Christoffer Wärnbring on 2021-06-07.
//

#include "Spaceship.h"

Spaceship::Spaceship(double mass) : Attractor(this, mass), Collidable(this, mass) {
    this->isplayer = true;
}

void Spaceship::update(double deltatime) {
    Attractor::update(deltatime);
    Object::update(deltatime);
    Collidable::update(deltatime);
}