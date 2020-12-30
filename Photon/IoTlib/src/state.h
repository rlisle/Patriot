/**
State

This class represents a state or condition, such as "watching tv",
"night", or "backdooropen" where value = 0 off, 100 on.
 It is also the parent class of Device
 It can represent a binary entity using 0 and 100 (or > 0)
It could be a measurement such as KitchenTemp where the
value is 20 to +127, or a percentage 0 - 100.

A state has a previous value that is reset at the beginning of each loop.


http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

*/
#pragma once

#include "application.h"  // Defines String

class State
{
protected:
    State*    _next;
    String    _name;
    int       _value;
    int       _previous;

    static State* _states = NULL;
    
public:
    State(String name, int value);

    int     value();
    void    setValue(int value);
    
    bool    hasChanged();
    
    // Collection methods (previously in States)
    static State* addState(String name, int value); // Add stated to linked list
    static State* getStateWithName(String name);    // Find state in linked list
    static int    count();                          // Number of State objects in linked list
    static void   syncPrevious();                   // Set this and all linked states _previous == _value
    
    /**
     Particle.io variable "States"
     */
    static void buildStatesVariable();
    static void expose();
};
