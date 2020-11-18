/******************************************************************
Behavior

This class represents a behavior, which is a response to one or
more states such as "tv" or "night".

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2020-11-18: Add conditions
2020-11-14: Rename activities to states
2017-10-22: Convert to scenes-like behavior
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle lib
2016-07-24: Initial version
******************************************************************/
#include "behavior.h"

Behavior::Behavior(int level)
{
    _level = level;
    _conditions = new Conditions();
}

void Behavior::addCondition(Condition *condition) {
    _conditions->addCondition(condition);
}

int Behavior::evaluateStates(States *states) 
{
    for(int x=0; x<_conditions->count(); x++){
        Condition* condition = _conditions->getCondition(x);
        if(condition->isTrue(states) == false) {
            return 0;
        }
    }
    return _level;
}
