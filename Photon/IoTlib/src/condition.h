/******************************************************************
Condition

This class represents a comparison to a state.
This is used by a Behavior to determine when to trigger.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2020-11-18: Initial version
******************************************************************/
#pragma once

#include "states.h"

class Condition
{
private:
    Condition *next;    // Linked list of conditions
    String _stateName;
    char _comparison;  // '<', '=', or '>'
    int _value;

public:
    Condition(String stateName, char comparison, int value);

    bool isTrue(States *states);
};
