/******************************************************************
Behaviors

This class represents a collection of Behavior objects.
Behaviors are things that happen when activites start or stop.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle lib
2016-09-09: Initial version
******************************************************************/
#pragma once

#include "behavior.h"

#define MAX_NUM_BEHAVIORS 64

class Behaviors
{
public:

    Behaviors();

    int addBehavior(Behavior *behavior); // Returns # behaviors (index+1)
    void performActivities(Activities *activities);
    int determineLevelForActivities(Device *device, int defaultPercent, Activities *activities);

private:
    uint8_t _numBehaviors = 0;
    Behavior *_behaviors[MAX_NUM_BEHAVIORS];
};
