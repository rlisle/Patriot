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
    _conditions = NULL;
}

void Conditions::addCondition(Condition *condition)
{
//    Serial.println("addCondition stateName: "+String(condition->_stateName));
    if(_conditions == NULL) {
//        Serial.println("  first condition");
        _conditions = condition;
    } else {
//        Serial.println("  add condition");
        Condition *ptr = _conditions;
        while(ptr->_next != NULL) {
//            Serial.println("  advance to next");
            ptr = ptr->_next;
        }
        ptr->_next = condition;
    }
}

Condition* Conditions::getCondition(int index) {
//    Serial.println("getCondition: "+String(index));
    Condition *ptr = _conditions;
    for (int i = index; i > 0 && ptr != NULL; i--) 
    {
//        Serial.println("  getting next, current = "+ptr->_stateName);
        ptr = ptr->_next;
    }
//    Serial.println("Returning ptr to "+ptr->_stateName);
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
