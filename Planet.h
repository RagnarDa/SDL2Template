//
// Created by Christoffer Wärnbring on 2021-06-06.
//

#ifndef SDLTEMPLATE_PLANET_H
#define SDLTEMPLATE_PLANET_H


#include "Collidable.h"
#include "Attractor.h"
#include "Mergeable.h"

const double planetmass = 1.0/1000000000000000000.0;
class Planet : public Object, public Attractor, public Collidable, public Mergeable {
public:
    Planet():Planet(planetmass){};
    Planet(double mass);
    void update(double deltatime);
    double mass;
};


#endif //SDLTEMPLATE_PLANET_H
