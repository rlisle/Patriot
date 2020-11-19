/******************************************************************
Conditions

This class represents a collection of Conditions objects.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2016-11-18: Initial version
******************************************************************/
#pragma once

#include "condition.h"

class Condition;

class Conditions
{
public:

    Conditions();

    void       addCondition(Condition *condition);
    Condition* getCondition(int index);
    int        count();

private:
    Condition* _conditions; // Head of singly linked list
};
