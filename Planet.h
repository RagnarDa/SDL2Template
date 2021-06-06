//
// Created by Christoffer Wärnbring on 2021-06-06.
//

#ifndef SDLTEMPLATE_PLANET_H
#define SDLTEMPLATE_PLANET_H


#include "Collidable.h"
#include "Attractor.h"

class Planet : public Object, public Attractor, public Collidable {
public:
    Planet(double mass);
    void update(double deltatime);
};


#endif //SDLTEMPLATE_PLANET_H
