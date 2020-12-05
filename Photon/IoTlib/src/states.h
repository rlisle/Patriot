/**
States

This class represents a collection of State objects.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.
*/
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
