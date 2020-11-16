/******************************************************************
Behavior

This class represents a behavior, which is a response to a state
such as "tv" or "night". Multiple states can be specified, but
currently absent states (eg. this but not that) are not supported.
These may be easily added in the future.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2020-11-14: Rename activities to states
2017-10-22: Convert to scenes-like behavior
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle lib
2016-07-24: Initial version
******************************************************************/
#include "behavior.h"

Behavior::Behavior(String stateName, char comparison, int value, int level)
{
    this->stateName = stateName;
    _comparison = comparison;     // '<', '=', '>', '!'
    _value = value;
    this->level = level;
}

bool Behavior::matchesCondition(int value)
{
    switch (_comparison)
    {
        case '<':
            if (value < _value)
            {
                return true;
            }
            break;
        case '=':
            if (value == _value)
            {
                return true;
            }
            break;
        case '>':
            if (value > _value)
            {
                return true;
            }
            break;
        case '!':
            if (value != _value)
            {
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

int Behavior::evaluateStates(States *states) {

    Serial.print("evaluateStates: " + stateName + " ");

    State *state = states->getStateWithName(stateName);
    if(state != NULL) {
        Serial.print("found: ");
        if (matchesCondition(state->_value))
        {
            Serial.println(String(level));
            return level;
        }
    }
    Serial.println("0");
    return 0;
}
