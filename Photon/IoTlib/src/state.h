/**
State

This class represents a state or condition, such as "watching tv",
"night", or "backdooropen" where value = 0 off, 100 on.
Or it could be a measurement such as KitchenTemp where the
value is 20 to +127, or a percentage 0 - 100.

A state can receive a start, stop, on, off, or value message:
<state>:0     (eg. tv:0)
<state>:100   (was "on", eg. "Booth Lamp:On")
<state>:value (eg. KitchenTemp:85


http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

*/
#pragma once

#include "application.h"  // Defines String

class States;

class State
{
    friend    States;
    
private:
    State*    _next;
    
public:
    String    _name;
    int       _value;
    
    State(String name, int value);
};
