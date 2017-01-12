/******************************************************************
 Activities

 This class represents a collection of Activity objects.

 http://www.github.com/rlisle/rv-arduino-libraries

 Written by Ron Lisle, ron@lisles.net

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2016-07-24: Initial version
 2016-09-09: Add behaviors
 ******************************************************************/

#ifndef activities_h
#define activities_h

#include "activity.h"

#define MAX_NUM_ACTIVITIES 64

class Activities
{
public:

  Activities();

  Activity*  addActivity(String name, int value);
  Activity*  getActivityByNum(int activityNum);
  Activity*  getActivityWithName(String name);
  int        count();

  bool       expose();

private:
  uint8_t     _numActivities;
  Activity*   _activities[MAX_NUM_ACTIVITIES];
  bool        _isVariableExposed;
  String      *_ptrToVariable;

  void buildActivityVariable();
};

#endif /* activities_h */
