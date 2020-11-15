/******************************************************************
Behavior

This class represents a behavior that occurs in response to one
or more activities, such as "tv" or "piano"

level will be set if event value is _comparison to _value
eg. event temperature value = 60, behavior value 70 '<' will set level.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2020-11-14: Rename activities to states
2017-10-22: Convert to "scenes" approach
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle lib
2016-07-07: Remove dimming mode
2016-01-17: Initial version
******************************************************************/
#pragma once

#include "states.h"

class Behavior
{
private:
    //TODO: convert these fields to a Trigger class
    char _comparison;  // '<', '=', or '>'
    int _value;

    bool matchesCondition(int value);

public:
    String stateName;
    int level;       // level to set if event <comparison> value is true

    Behavior(String stateName, char comparison, int value, int level);

    int evaluateStates(States *states);
};
