/******************************************************************
 behavior

 This class represents a behavior, which is a response to an activity
 such as "tv" or "piano". Multiple activities can be specified, but
 currently absent activities (eg. this but not that) are not supported.
 These may be easily added in the future.

 http://www.github.com/rlisle/rv-arduino-libraries

 Written by Ron Lisle, ron@lisles.net

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Changelog:
 2016-07-24: Initial version
 ******************************************************************/

#include "behavior.h"

Behavior::Behavior(String activityName, char comparison, int value, int lvl)
{
  _activityName = activityName;
  _comparison   = comparison;     // '<', '=', '>', '!'
  _value        = value;
  level        = lvl;
}

bool Behavior::matchesCondition(String name, int value)
{
  if(name.equalsIgnoreCase(_activityName))
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
      if(matchesCondition(activity->name, activity->value))
      {
          return level;
      }
  }
  return 0;
}
