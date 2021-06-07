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
            // Possible bugfix, if we use position of next step we might get around interaction effect
            // That didnt have any effect...
            double anposX = a->pobject->posX + (a->pobject->movementworldX * deltatime);
            double anposY = a->pobject->posY + (a->pobject->movementworldY * deltatime);
            double anposZ = a->pobject->posZ + (a->pobject->movementworldZ * deltatime);
            double bnposX = this->pobject->posX + (this->pobject->movementworldX * deltatime);
            double bnposY = this->pobject->posY + (this->pobject->movementworldY * deltatime);
            double bnposZ = this->pobject->posZ + (this->pobject->movementworldZ * deltatime);
            double dx = anposX - bnposX;
            double dy = anposY - bnposY;
            double dz = anposZ - bnposZ;
            double dist = std::sqrt(dx*dx+dy*dy+dz*dz);
            double magi = a->mass/(dist*dist*dist);
            this->pobject->movementworldX += magi * dx * deltatime;
            this->pobject->movementworldY += magi * dy * deltatime;
            this->pobject->movementworldZ += magi * dz * deltatime;
        }
    }
}
