/******************************************************************
Behaviors

This class represents a collection of Behavior objects.
Behaviors are things that happen when activites start or stop.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2020-11-14: Rename activites to states.
2017-10-22: Changed to perform like "scenes".
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle lib
2016-09-09: Initial version
******************************************************************/
#pragma once

#include "behavior.h"

class Behavior;

class Behaviors
{
public:

    Behaviors();

    void      addBehavior(Behavior *behavior);
//    Behavior* getBehavior(String name);
    int       stateDidChange(States *states);  // Returns calculated value
    int       count();
    Behavior* getBehaviorAt(int index);

private:
    Behavior* _behaviors;
};
