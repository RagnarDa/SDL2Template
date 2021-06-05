//
// Created by Christoffer Wärnbring on 2021-06-05.
//

#include "Attractor.h"
#include <vector>
#include <cmath>

Attractor::Attractor(double mass):mass(mass) {
}

void Attractor::update(double deltatime) {
    for (auto a : *pattractors)
    {
        if (a != this)
        {
            double dx = a->posX - this->posX;
            double dy = a->posY - this->posY;
            double dz = a->posZ - this->posZ;
            double dist = std::sqrt(dx*dx+dy*dy+dz*dz);
            double magi = a->mass/(dist*dist*dist);
            this->movementX += magi*dx*deltatime;
            this->movementY += magi*dy*deltatime;
            this->movementZ += magi*dz*deltatime;
        }
    }
    Object::update(deltatime);
}
