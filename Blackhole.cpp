//
// Created by Christoffer Wärnbring on 2021-06-08.
//

#include "Blackhole.h"

Blackhole::Blackhole(double mass) : Attractor(this, mass), Consumer(this, mass) {}

void Blackhole::update(double deltatime) {
    if (!draw)
        return;
    Attractor::update(deltatime);
    Object::update(deltatime);
    Consumer::update(deltatime);
}