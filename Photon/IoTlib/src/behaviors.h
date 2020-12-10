/**
Behaviors

This class represents a collection of Behavior objects.
Behaviors are things that happen when activites start or stop.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

*/
#pragma once

#include "behavior.h"

class Behavior;

class Behaviors
{
public:

    Behaviors();

    void      addBehavior(Behavior *behavior);
    int       stateDidChange(States *states);  // Returns calculated value
    int       count();
    Behavior* getBehaviorAt(int index);

private:
    Behavior* _behaviors;
};
