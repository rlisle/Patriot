/**
Behavior

This class represents a behavior that occurs in response to one
or more activities, such as "tv" or "piano"

level will be set if event value is _comparison to _value
eg. event temperature value = 60, behavior value 70 '<' will set level.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

*/
#pragma once

#include "states.h"
#include "conditions.h"

class Behaviors;

class Behavior
{
    friend Behaviors;
    
private:
    Behavior*   _next;
    Conditions* _conditions;
    int         _level;       // level to set if event <comparison> value is true
    bool        _isDefault;     // Set for auto behavior set by direct commands (Alexa).

public:
    Behavior(int level, bool isDefault = false);

    void addCondition(Condition *condition);
    int  evaluateStates(States *states);
    Conditions *getConditions() { return _conditions; }
};
