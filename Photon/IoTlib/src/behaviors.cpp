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
2020-11-14: Rename activities to states.
2017-10-22: Convert to scenes-like behavior.
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle lib
2016-06-24: Initial version
******************************************************************/
#include "behaviors.h"

Behaviors::Behaviors()
{
    // Without this method, strange error is reported and build fails
    //TODO: Restore activities from EEPROM
    //size_t len = EEPROM.length();
    //Particle.publish("behaviors","EEPROM length is: "+String(len), 60, PRIVATE);
//    char numActivities = EEPROM[0];
}


//TODO: convert to linked list
void Behaviors::addBehavior(Behavior *behavior)
{
    if (_numBehaviors < MAX_NUM_BEHAVIORS - 1)
    {
        _behaviors[_numBehaviors++] = behavior;
    }
}

int Behaviors::stateDidChange(States *states)
{
    int level = 0;
    for (int i = 0; i < _numBehaviors; i++)
    {
        Behavior *behavior = _behaviors[i];
        int newLevel = behavior->evaluateStates(states);
        level = max(level,newLevel);
    }
    return level;
}
