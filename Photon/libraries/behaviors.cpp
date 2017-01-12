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

 http://www.github.com/rlisle/rv-arduino-libraries

 Written by Ron Lisle, ron@lisles.net
 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Changelog:
 2016-06-24: Initial version
 ******************************************************************/

#include "behaviors.h"

Behaviors::Behaviors() {
    // Without this method, strange error is reported and build fails
    //TODO: Restore activities from EEPROM
    //size_t len = EEPROM.length();
    //Particle.publish("behaviors","EEPROM length is: "+String(len));
//    char numActivities = EEPROM[0];
}

// Returns # behaviors (index+1 of added behavior, or -1 if array already full)
int Behaviors::addBehavior(Behavior *behavior)
{
  if (_numBehaviors < MAX_NUM_BEHAVIORS-1)
  {
    _behaviors[_numBehaviors++] = behavior;
  } else {
    return -1;
  }
  //TODO: Write out all behaviors to EEPROM

  return _numBehaviors-1;
}

// void Behaviors::performBehaviorsForActivity(Activity *activity) {
//   for(int i=0; i<_numBehaviors; i++)
//   {
//     //TODO:
//       /*if(_activities[i]->name.equalsIgnoreCase(name)) {
//           _activities[i]->performAction(state);
//       }*/
//   }
// }
//
// void Behaviors::performBehaviorsForActivities(Activities *activities) {
//
// }

// int Behaviors::determineLevelForActivity(String name, int value) {
//   int level = -1; // -1 means no matches for name, value
//   for(int i = 0; i < _numBehaviors; i++) {
//     Behavior* behavior = _behaviors[i];
//     if(behavior->matchesCondition(name, value)) {
//       return behavior->level;
//     }
//   }
//   return level; // -1 means no matches for name, value
// }
//
int Behaviors::determineLevelForActivities(int defaultPercent, Activities *activities) {
  int level = defaultPercent;
  for(int i = 0; i < _numBehaviors; i++) {
    Behavior* behavior = _behaviors[i];
    int newLevel = behavior->determineLevel(activities);
    // For now, simply select the highest level set by any behavior
    // Later we may want to factor priorities, etc. into the equation
    if( newLevel > level) level = newLevel;
  }
  return level;
}
