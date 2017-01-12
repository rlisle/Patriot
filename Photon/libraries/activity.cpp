/******************************************************************
 activity

 http://www.github.com/rlisle/rv-arduino-libraries

 Written by Ron Lisle, ron@lisles.net

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2016-07-24: Initial version
 ******************************************************************/

#include "activity.h"

Activity::Activity(String nm, int val)
{
  value = val;
  name = nm;
}
