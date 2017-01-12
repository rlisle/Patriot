/******************************************************************
 Behavior

 This class prepresents a behavior that occurs in response to one
 or more activities, such as "tv" or "piano"

 level will be set if event value is _comparison to _value
 eg. event temperature value = 60, behavior value 70 '<' will set level.

 http://www.github.com/rlisle/rv-arduino-libraries

 Written by Ron Lisle, ron@lisles.net

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2016-07-07: Remove dimming mode
 2016-01-17: Initial version
 ******************************************************************/

#ifndef behavior_h
#define behavior_h

#include "activities.h"

class Activities;

class Behavior
{
private:
  //TODO: use collection of activities to allow multiple
  String      _activityName;
  char        _comparison;  // '<', '=', or '>'
  int         _value;

public:
  int         level;       // level to set if event <comparison> value is true
  bool        matchesCondition(String name, int value);
  Behavior(String activityName, char comparison, int value, int level);

  int       determineLevel(Activities *activities);
};

#endif /* activity_h */
