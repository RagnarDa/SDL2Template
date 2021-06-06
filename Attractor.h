//
// Created by Christoffer Wärnbring on 2021-06-05.
//

#ifndef SDLTEMPLATE_ATTRACTOR_H
#define SDLTEMPLATE_ATTRACTOR_H


#include "Object.h"
#include <vector>

class Attractor: public Object {
public:
    Attractor(double mass);
    void update(double deltatime);
    std::vector<Attractor*> * pattractors;
    double mass;
};


#endif //SDLTEMPLATE_ATTRACTOR_H
