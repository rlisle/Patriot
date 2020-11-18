/******************************************************************
Conditions

This class represents a collection of Conditions objects.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle
BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2020-11-18: Initial version
******************************************************************/
#include "conditions.h"

Conditions::Conditions()
{
    // Without this method, strange error is reported and build fails
}

void Conditions::addCondition(Condition *condition)
{
    if(_conditions == NULL) {
        _conditions = condition;
    } else {
        Condition *ptr = _conditions;
        while(ptr->_next != NULL) {
            ptr = ptr->_next;
        }
        ptr->_next = condition;
    }
}

Condition* Conditions::getCondition(int index) {
    Condition *ptr = _conditions;
    for (int i = index; i > 0 && ptr != NULL; i--) 
    {
        ptr = ptr->_next;
    }
    return ptr;
}
