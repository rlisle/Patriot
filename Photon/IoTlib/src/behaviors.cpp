/******************************************************************
Behaviors collection

This object manages a collection of Behavior objects.
A Behavior object describes the response of a device
to received "activity notifications".
Activities are received via Particle.io Pub/Sub.

TODO:
1. persist behaviors to EEPROM
  - Photon has 2k EEPROM (actually, flash working like EEPROM)
  - So can hold 128 x 16 byte activity structures per device

http://www.github.com/rlisle/Patriot

Written by Ron Lisle
BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2017-10-22: Convert to "scenes" behavior.
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle lib
2016-06-24: Initial version
******************************************************************/
#include "IoT.h"
#include "behaviors.h"

Behaviors::Behaviors()
{
    // Without this method, strange error is reported and build fails
    //TODO: Restore activities from EEPROM
    //size_t len = EEPROM.length();
    //Particle.publish("behaviors","EEPROM length is: "+String(len), 60, PRIVATE);
//    char numActivities = EEPROM[0];
}


// Returns # behaviors (index+1 of added behavior, or -1 if array already full)
int Behaviors::addBehavior(Behavior *behavior)
{
    if (_numBehaviors < MAX_NUM_BEHAVIORS - 1)
    {
        IoT::log("Adding behavior");
        _behaviors[_numBehaviors++] = behavior;
    } else
    {
        IoT::log("Max # behaviors exceeded");
        return -1;
    }
    //TODO: Write out all behaviors to EEPROM

    return _numBehaviors - 1;
}


void performActivity(String name, int value)
{
    Serial.println("Behaviors performing activity "+name+" = "+String(value));

    for (int i = 0; i < _numBehaviors; i++)
    {
        Behavior *behavior = _behaviors[i];
        behavior->performActivity(name, level);
    }
}
