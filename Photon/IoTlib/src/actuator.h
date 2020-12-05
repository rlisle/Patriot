/**
*  Actuator.h
*  Patriot
*
*  Actuators can be attached to Devices (and Activities)
*  to control other Devices (and Activities)
*  This is mainly needed to allow one Activity to turn off another
*
*  Created by Ron on 12/4/20.
*/
#pragma once

#include "application.h"    // Defines String

class Actuators;
class Device;

class Actuator
{
    friend Actuators;
    friend Device;
    
private:
    Actuator* _next;
    
public:
    String  _name;
    int     _value;
    
    Actuator(String name, int value);
};
