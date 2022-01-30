//
// Created by Christoffer Wärnbring on 2021-06-07.
//

#ifndef SDLTEMPLATE_SPACESHIP_H
#define SDLTEMPLATE_SPACESHIP_H


#include "Collidable.h"
#include "Attractor.h"

class Spaceship : public Object, public Attractor, public Collidable {
public:
    Spaceship(double mass);
    void update(double deltatime);
    double mass;
};


#endif //SDLTEMPLATE_SPACESHIP_H
