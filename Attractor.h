//
// Created by Christoffer Wärnbring on 2021-06-05.
//

#ifndef SDLTEMPLATE_ATTRACTOR_H
#define SDLTEMPLATE_ATTRACTOR_H


#include "Object.h"
#include <vector>

class Attractor{
public:
    Attractor(Object * obj, double * mass);
    void update(double deltatime);
    std::vector<Attractor*> * pattractors;
    double * mass;
    Object * pobject;
};


#endif //SDLTEMPLATE_ATTRACTOR_H
