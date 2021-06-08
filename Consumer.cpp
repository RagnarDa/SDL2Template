//
// Created by Christoffer Wärnbring on 2021-06-08.
//

#include "Consumer.h"

#include <cmath>

Consumer::Consumer(Object * obj, double mass):pobject(obj), _mass(mass) {
}

void Consumer::update(double deltatime) {
    for (auto a : *pobjects)
    {
        if (a != this->pobject)
        {
            // Only 2D distance
            double dy = a->posY - this->pobject->posY;
            double dz = a->posZ - this->pobject->posZ;
            double dist = std::sqrt(dy*dy+dz*dz);
            const double blackholesize = 1.0;
            if (dist < blackholesize)
            {
                a->movementworldX += (dist/blackholesize)/deltatime;
                a->movementworldY *= (dist/blackholesize)*deltatime;
                a->movementworldZ *= (dist/blackholesize)*deltatime;
//                a->movementworldY *= (blackholesize-dist)-deltatime;
//                a->movementworldZ *= (blackholesize-dist)-deltatime;
            }
        }
    }
}