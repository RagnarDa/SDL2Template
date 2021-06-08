//
// Created by Christoffer Wärnbring on 2021-06-08.
//

#ifndef SDLTEMPLATE_CONSUMER_H
#define SDLTEMPLATE_CONSUMER_H

#include "Object.h"
#include <vector>

class Consumer{
public:
    Consumer(Object * obj, double mass);
    void update(double deltatime);

    double _mass;
    std::vector<Object*> * pobjects;
    Object * pobject;
    static int getscore();
};


#endif //SDLTEMPLATE_CONSUMER_H
