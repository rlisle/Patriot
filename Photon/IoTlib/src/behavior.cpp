/******************************************************************
behavior

This class represents a behavior, which is a response to an activity
such as "tv" or "piano". Multiple activities can be specified, but
currently absent activities (eg. this but not that) are not supported.
These may be easily added in the future.

http://www.github.com/rlisle/ParticleIoT

Written by Ron Lisle, ron@lisles.net

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-05: Convert to v2 particle lib
2016-07-24: Initial version
******************************************************************/
#include "behavior.h"

Behavior::Behavior(String activityName, char comparison, int value, int level)
{
  this->activityName  = activityName;
  _comparison        = comparison;     // '<', '=', '>', '!'
  _value             = value;
  this->level         = level;
}

bool Behavior::matchesCondition(String name, int value)
{
  Serial.println("matchesCondition? "+name+String(value));
  if(name.equalsIgnoreCase(activityName))
  {
    switch(_comparison) {
      case '<':
      if(value < _value){
        return true;
      }
      break;
      case '=':
      if(value == _value)
      {
        return true;
      }
      break;
      case '>':
      if(value > _value)
      {
        return true;
      }
      break;
      case '!':
      if(value != _value)
      {
        return true;
      }
      break;
      default:
      break;
    }
  }
  return false;
}

int Behavior::determineLevel(Activities *activities)
{
  for(int i = 0; i < activities->count(); i++)
  {
      Activity *activity = activities->getActivityByNum(i);
      if(matchesCondition(activity->_name, activity->_value))
      {
          return level;
      }
  }
  return 0;
}
