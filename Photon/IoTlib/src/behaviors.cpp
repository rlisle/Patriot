/******************************************************************
Behaviors collection

This object manages a collection of Behavior objects.
A Behavior object describes the response of a device
to received "activity notifications".
Activities are received via Particle.io Pub/Sub.

Multiple activities can be active at the same time, so it is
important to combine their effects, in addition to stopping an
activity in a manner that doesn't break activities that continue
to be in effect.

When an activity is started, its name is added to an array of
active activities. When the activity is stopped, its name is
removed from the array. The activity array is passed to the
performBehaviors() function whenever there is a change.

TODO:
1. persist behaviors to EEPROM
  - Photon has 2k EEPROM (actually, flash working like EEPROM)
  - So can hold 128 x 16 byte activity structures per device

http://www.github.com/rlisle/Patriot

Written by Ron Lisle
BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
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
    //Particle.publish("behaviors","EEPROM length is: "+String(len));
//    char numActivities = EEPROM[0];
}


// Returns # behaviors (index+1 of added behavior, or -1 if array already full)
int Behaviors::addBehavior(Behavior *behavior)
{
    if (_numBehaviors < MAX_NUM_BEHAVIORS - 1)
    {
        _behaviors[_numBehaviors++] = behavior;
    } else
    {
        return -1;
    }
    //TODO: Write out all behaviors to EEPROM

    return _numBehaviors - 1;
}


int Behaviors::determineLevelForActivities(Device *device, int defaultPercent, Activities *activities)
{
    Serial.println("determineLevelForActivities: " + String(_numBehaviors) + " behaviors");
    int level = defaultPercent;
    for (int i = 0; i < _numBehaviors; i++)
    {
        Serial.println("checking behavior " + String(i));
        Behavior *behavior = _behaviors[i];
        if (behavior->device == device)
        {
            int newLevel = behavior->determineLevel(activities);
            // For now, simply select the highest level set by any behavior
            // Later we may want to factor priorities, etc. into the equation
            if (newLevel > level)
            {
                level = newLevel;
            }
        }
    }
    Serial.println("   return " + String(level));
    return level;
}
