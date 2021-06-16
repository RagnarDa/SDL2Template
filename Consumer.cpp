//
// Created by Christoffer Wärnbring on 2021-06-08.
//

#include "Consumer.h"

#include <cmath>
int score = 0;

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
            const double eventhorizon = 1.0;
            if (dist < eventhorizon)
            {
                a->movementworldX += deltatime * 100.0;
//                a->movementworldY -= dy * deltatime * 0.5;
//                a->movementworldZ -= dz * deltatime * 0.5;
                a->posY -= dy * deltatime;
                a->posZ -= dz * deltatime;
//                a->movementworldY += this->pobject->movementworldY * deltatime;
//                a->movementworldZ += this->pobject->movementworldZ * deltatime;
//                a->movementworldY *= (eventhorizon-dist)-deltatime;
//                a->movementworldZ *= (eventhorizon-dist)-deltatime;
            }
                if (a->posX > 150) {
                    for (int o = 0; o<pobjects->size(); o++)
                    {
                        if (pobjects->at(o) == a) {
                            // erase the nth element
                            pobjects->erase(pobjects->begin() + o);
                            a->draw = false;
                            score++;
                        }
                    }
                }
        }
    }
}

int Consumer::getscore() {
    return score;
}
