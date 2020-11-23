/******************************************************************
States

This class represents a collection of State objects.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle library
2016-07-24: Initial version
2016-09-09: Add behaviors
******************************************************************/
#pragma once

#include "state.h"

class States
{
public:
    
    States();
    
    State*     addState(String name, int value);
    State*     getStateWithName(String name);
    int        count();
    
private:
    State*     _states;                 // Linked list
    
    void buildStatesVariable();
    void expose();

};
