/******************************************************************
Behaviors collection

This object manages a collection of Behavior objects.
A Behavior object describes the response of a device
to received "state notifications".
States are received via Particle.io Pub/Sub or MQTT.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle
BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2020-11-19: Convert to linked list
2020-11-14: Rename activities to states.
2017-10-22: Convert to scenes-like behavior.
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle lib
2016-06-24: Initial version
******************************************************************/
#include "behaviors.h"

Behaviors::Behaviors()
{
    _behaviors = NULL;
}

void Behaviors::addBehavior(Behavior *behavior)
{
//    Serial.println("Adding behavior");
    if(_behaviors == NULL) {
//        Serial.println("  first behavior");
        _behaviors = behavior;
    } else {
//        Serial.println("  add behavior");
        Behavior *ptr = _behaviors;
        while(ptr->_next != NULL) {
//            Serial.println("  advance to next");
            ptr = ptr->_next;
        }
        ptr->_next = behavior;
    }
}

int Behaviors::stateDidChange(States *states)
{
    int level = 0;
    for (Behavior *ptr = _behaviors; ptr != NULL; ptr = ptr->_next)
    {
        //Currently just picking highest level.
        int newLevel = ptr->evaluateStates(states);
        level = max(level,newLevel);
    }
    return level;
}
