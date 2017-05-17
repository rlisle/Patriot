/******************************************************************
Activities

This class represents a collection of Activity objects.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle library
2016-07-24: Initial version
2016-09-09: Add behaviors
******************************************************************/
#pragma once

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
