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
#include "condition.h"

Condition::Condition(String stateName, char comparison, int value)
{
    _stateName = stateName;
    _comparison = comparison;     // '<', '=', '>', '!'
    _value = value;
    _next = NULL;
}

bool Condition::matchesCondition(int stateValue)
{
    switch (_comparison)
    {
        case '<':
            return stateValue < _value;
        case '=':
            return stateValue == _value;
        case '>':
            return stateValue > _value;
        case '!':
            return stateValue != _value;
        default:
            break;
    }
    return false;
}

bool Condition::isTrue(States *states) {

    State *state = states->getStateWithName(_stateName);
    if(state != NULL) {
        Serial.print("State found: ");
        return matchesCondition(state->_value);
    }
    Serial.println("false");
    return false;
}
