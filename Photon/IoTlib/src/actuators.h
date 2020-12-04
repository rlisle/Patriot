/**
 actuators.h
 
 This class represents a collection of Actuator objects.

 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle

 BSD license, check LICENSE for more information.
 All text above must be included in any redistribution.
*/

#pragma once

#include "actuator.h"

class Actuators
{
public:
    
    Actuators();
    
    Actuator*   addActuator(String name, int value);
    Actuator*   getActuatorWithName(String name);
    int         count();
    
private:
    Actuator*   _actuators;         // Linked list
    
};
