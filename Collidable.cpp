//
// Created by Christoffer Wärnbring on 2021-06-06.
//

#include "Collidable.h"
#include <cmath>
#include <cassert>
bool collisionhasoccured = false;

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
                //this->pobject->posX += fOverlap * (a->pobject->posX - this->pobject->posX) / dist;
                //this->pobject->posY += fOverlap * (a->pobject->posY - this->pobject->posY) / dist;
                //this->pobject->posZ += fOverlap * (a->pobject->posZ - this->pobject->posZ) / dist;

                // Dynamic collision
                // Normal
                double nx = (this->pobject->posX - a->pobject->posX) / dist;
                double ny = (this->pobject->posY - a->pobject->posY) / dist;
                double nz = (this->pobject->posZ - a->pobject->posZ) / dist;
// Wikipedia Version - Maths is smarter but same
//float kx = (b1->vx - b2->vx);
//float ky = (b1->vy - b2->vy);
//float p = 2.0 * (nx * kx + ny * ky) / (b1->mass + b2->mass);
//b1->vx = b1->vx - p * b2->mass * nx;
//b1->vy = b1->vy - p * b2->mass * ny;
//b2->vx = b2->vx + p * b1->mass * nx;
//b2->vy = b2->vy + p * b1->mass * ny;

// Recalculate dist
                dx = a->pobject->posX - this->pobject->posX;
                dy = a->pobject->posY - this->pobject->posY;
                dz = a->pobject->posZ - this->pobject->posZ;
                double p = 2.0 * (nx * dx + ny * dy + nz * dz) / (a->_mass + this->_mass);
                p *= 0.1; // Softness
                this->pobject->movementworldX = this->pobject->movementworldX - p * a->_mass * nx;
                this->pobject->movementworldY = this->pobject->movementworldY - p * a->_mass * ny;
                this->pobject->movementworldZ = this->pobject->movementworldZ - p * a->_mass * nz;
     //           a->pobject->movementworldX = a->pobject->movementworldX - p * this->_mass * nx;
       //         a->pobject->movementworldY = a->pobject->movementworldY - p * this->_mass * ny;
         //       a->pobject->movementworldZ = a->pobject->movementworldZ - p * this->_mass * nz;

                // Impart spin
                // http://hyperphysics.phy-astr.gsu.edu/hbase/mi.html
                // Calculate moment of inertia
                // Assume sphere
                double I_a = (2.0/5.0)*a->_mass*radia*radia;
                double I_b = (2.0/5.0)*this->_mass*radib*radib;
                double tanva = radia * a->pobject->movementrotation;
                double tanvb = radib * this->pobject->movementrotation;
                double A_a = tanva - tanvb;
                double A_b = tanvb - tanva;
                double T_a = I_a * A_a;
                double T_b = I_a * A_b;
                double F_a = T_a/radia * 1.0;
                double F_b = T_b/radib * 0.1;
                // Let's do some simple un-smart trigonometry, notice it is turned around X axis
                // so Y and Z is swapped
                vector a_to_b(std::array<double,3>{this->pobject->posX-a->pobject->posX
                , this->pobject->posZ-a->pobject->posZ
                , this->pobject->posY-a->pobject->posY});

                this->pobject->movementworldY -= a_to_b.Y() * F_b;
                this->pobject->movementworldZ += a_to_b.Z() * F_b;
          //      a->pobject->movementworldY += a_to_b.Y() * F_b;
          //      a->pobject->movementworldZ += a_to_b.Z() * F_b;

                // Hacky but seems to work
                // circumference = radius * pi * 2
                double prot = 2.0 * (tanva + tanvb) / (a->_mass + this->_mass);
                prot *= 1.0;
                this->pobject->movementrotation -= prot * a->_mass;
                //a->pobject->movementrotation -= prot * this->_mass;

                // For playing sound
                collisionhasoccured = true;
            }
        }
    }
}


/// <summary>
/// Has occured since last call to this function
/// </summary>
/// <returns></returns>
bool Collidable::CollisionHasOccured()
{
    bool hasoc = collisionhasoccured;
    collisionhasoccured = false;
    return hasoc;
}
