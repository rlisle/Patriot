/**
Behaviors collection

This object manages a collection of Behavior objects.
A Behavior object describes the response of a device
to received "state notifications".
States are received via Particle.io Pub/Sub or MQTT.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle
BSD license, check license.txt for more information.
All text above must be included in any redistribution.

*/
#include "behaviors.h"

Behaviors::Behaviors()
{
    _behaviors = NULL;
}

void Behaviors::addBehavior(Behavior *behavior)
{
    Log.trace("Adding behavior");
    if(_behaviors == NULL) {
        _behaviors = behavior;
    } else {
        Behavior *ptr = _behaviors;
        while(ptr->_next != NULL) {
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
        Log.trace("stateDidChange");
        //Currently just picking highest level.
        int newLevel = ptr->evaluateStates(states);
        level = max(level,newLevel);
    }
    return level;
}

int Behaviors::count()
{
    int cnt = 0;
    for(Behavior *ptr = _behaviors; ptr!=NULL; ptr = ptr->_next, cnt++);
    return cnt;
}

Behavior *Behaviors::getBehaviorAt(int index)
{
    Behavior *ptr = _behaviors;
    for(int x = 0; x<index && ptr->_next !=NULL; x++) {
        ptr = ptr->_next;
    }
    return ptr;
}
