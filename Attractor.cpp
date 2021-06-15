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
        if (a->pobject != this->pobject && this->pobject->draw)
        {
            double dx = a->pobject->posX - this->pobject->posX;
            double dy = a->pobject->posY - this->pobject->posY;
            double dz = a->pobject->posZ - this->pobject->posZ;
            double dist = std::sqrt(dx*dx+dy*dy+dz*dz);
//            if (dx == 0 || dy == 0 || dz == 0 || dist == 0)
//                continue;
            double magi = a->mass/(dist*dist*dist);
            this->pobject->movementworldX += magi * dx * deltatime;
            this->pobject->movementworldY += magi * dy * deltatime;
            this->pobject->movementworldZ += magi * dz * deltatime;
            assert(!isnan(this->pobject->movementworldX));
            assert(!isnan(this->pobject->movementworldY));
            assert(!isnan(this->pobject->movementworldZ));
        }
    }
}
