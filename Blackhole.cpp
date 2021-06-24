//
// Created by Christoffer Wärnbring on 2021-06-08.
//

#include "Blackhole.h"

Blackhole::Blackhole(double mass) : mass(mass), Attractor(this, &this->mass), Consumer(this, &this->mass) {}

void Blackhole::update(double deltatime) {
    if (!draw)
        return;
    Attractor::update(deltatime);
    Object::update(deltatime);
    Consumer::update(deltatime);
}