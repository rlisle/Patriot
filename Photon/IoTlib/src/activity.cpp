/******************************************************************
activity

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle library
2016-07-24: Initial version
******************************************************************/
#include "activity.h"

Activity::Activity(String name, int value)
{
  Serial.print("New activity: ");
  Serial.println(name);
  _value = value;
  _name = name;
}
