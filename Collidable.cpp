//
// Created by Christoffer Wärnbring on 2021-06-06.
//

#include "Collidable.h"
#include <cmath>

Collidable::Collidable(Object * obj, double mass) : pobject(obj), _mass(mass){
    assert(mass>0);
}

void Collidable::update(double deltatime) {
    for (auto a : *pcollidables)
    {
        if (a->pobject != this->pobject)
        {
            double dx = a->pobject->posX - this->pobject->posX;
            double dy = a->pobject->posY - this->pobject->posY;
            double dz = a->pobject->posZ - this->pobject->posZ;
            double dist = std::sqrt(dx*dx+dy*dy+dz*dz);
            // Let's assume roundness right now
            double radia = a->pobject->sizeY/2;
            double radib = this->pobject->sizeY/2;
            if (dist < radia+radib)
            {
                // Collision code base on https://github.com/OneLoneCoder/videos/blob/master/OneLoneCoder_Balls1.cpp

                // First, static collision

                // Calculate displacement required
                float fOverlap = 0.5f * (dist - radia - radib);

                // Displace Current Ball away from collision
                a->pobject->posX -= fOverlap * (a->pobject->posX - this->pobject->posX) / dist;
                a->pobject->posY -= fOverlap * (a->pobject->posY - this->pobject->posY) / dist;
                a->pobject->posZ -= fOverlap * (a->pobject->posZ - this->pobject->posZ) / dist;

                // Displace Target Ball away from collision
                this->pobject->posX += fOverlap * (a->pobject->posX - this->pobject->posX) / dist;
                this->pobject->posY += fOverlap * (a->pobject->posY - this->pobject->posY) / dist;
                this->pobject->posZ += fOverlap * (a->pobject->posZ - this->pobject->posZ) / dist;

                // Dynamic collision
                // Normal
                double nx = (this->pobject->posX - a->pobject->posX) / dist;
                double ny = (this->pobject->posY - a->pobject->posY) / dist;
                double nz = (this->pobject->posZ - a->pobject->posZ) / dist;

                double p = 2.0 * (nx * dx + ny * dy + nz * dz) / (a->_mass + this->_mass);
                this->pobject->movementworldX = this->pobject->movementworldX - p * a->_mass * nx;
                this->pobject->movementworldY = this->pobject->movementworldY - p * a->_mass * ny;
                this->pobject->movementworldZ = this->pobject->movementworldZ - p * a->_mass * nz;
                a->pobject->movementworldX = a->pobject->movementworldX - p * this->_mass * nx;
                a->pobject->movementworldY = a->pobject->movementworldY - p * this->_mass * ny;
                a->pobject->movementworldZ = a->pobject->movementworldZ - p * this->_mass * nz;

                // Impart spin
                // Hacky but seems to work
                // circumference = radius * pi * 2
                double tanva = radia * a->pobject->movementrotation;
                double tanvb = radib * this->pobject->movementrotation;
                double prot = 2.0 * (tanva + tanvb) / (a->_mass + this->_mass);
                this->pobject->movementrotation -= prot * a->_mass;
                a->pobject->movementrotation -= prot * this->_mass;
            }
        }
    }
}
