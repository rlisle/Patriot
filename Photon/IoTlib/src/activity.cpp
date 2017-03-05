/******************************************************************
activity

http://www.github.com/rlisle/ParticleIoT

Written by Ron Lisle, ron@lisles.net

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-05: Convert to v2 particle library
2016-07-24: Initial version
******************************************************************/
#include "activity.h"

Activity::Activity(String name, int value)
{
  _value = value;
  _name = name;
}
