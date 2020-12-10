/**
Conditions

This class represents a collection of Conditions objects.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle
BSD license, check license.txt for more information.
All text above must be included in any redistribution.

*/
#include "conditions.h"

Conditions::Conditions()
{
    _conditions = NULL;
}

void Conditions::addCondition(Condition *condition)
{
    Log.info("addCondition stateName: "+String(condition->_stateName));
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
//    Log.info("getCondition: "+String(index));
    Condition *ptr = _conditions;
    for (int i = index; i > 0 && ptr->_next != NULL; i--)
    {
        ptr = ptr->_next;
    }
    return ptr;
}

int Conditions::count() {
    Condition *ptr = _conditions;
    int count=0;
    while(ptr != NULL){
        count++;
        ptr=ptr->_next;
    }
    return count;
}
