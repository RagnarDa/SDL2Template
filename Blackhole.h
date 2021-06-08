//
// Created by Christoffer Wärnbring on 2021-06-08.
//

#ifndef SDLTEMPLATE_BLACKHOLE_H
#define SDLTEMPLATE_BLACKHOLE_H


#include "Object.h"
#include "Attractor.h"
#include "Consumer.h"

class Blackhole : public Object, public Attractor, public Consumer {
public:
    Blackhole(double mass);
    void update(double deltatime);
};



#endif //SDLTEMPLATE_BLACKHOLE_H
