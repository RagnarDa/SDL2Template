//
// Created by Christoffer Wärnbring on 2021-06-05.
//

#include "Attractor.h"
#include <vector>
#include <cmath>

Attractor::Attractor(Object * obj, double mass):pobject(obj), mass(mass) {
}

void Attractor::update(double deltatime) {
    for (auto a : *pattractors)
    {
        if (a->pobject != this->pobject)
        {
            double dx = a->pobject->posX - this->pobject->posX;
            double dy = a->pobject->posY - this->pobject->posY;
            double dz = a->pobject->posZ - this->pobject->posZ;
            double dist = std::sqrt(dx*dx+dy*dy+dz*dz);
            double magi = a->mass/(dist*dist*dist);
            this->pobject->movementworldX += magi * dx * deltatime;
            this->pobject->movementworldY += magi * dy * deltatime;
            this->pobject->movementworldZ += magi * dz * deltatime;
        }
    }
}
