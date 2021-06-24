//
// Created by Christoffer Wärnbring on 2021-06-06.
//

#ifndef SDLTEMPLATE_COLLIDABLE_H
#define SDLTEMPLATE_COLLIDABLE_H


#include "Object.h"
#include <vector>

class Collidable{
public:
    Collidable(Object * obj, double * mass);
    void update(double deltatime);

    double * _mass;
    std::vector<Collidable*> * pcollidables;
    Object * pobject;
    static bool CollisionHasOccured();
};


#endif //SDLTEMPLATE_COLLIDABLE_H
