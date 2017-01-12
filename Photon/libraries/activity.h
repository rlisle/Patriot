/******************************************************************
 Activity

 This class prepresents an activity, such as "watching tv",
 "playing piano", or "BoothSw2" where value = 0 off, 100 on.
 Or it could be a measurement such as KitchenTemp where the
 value is 20 to +127, or a percentage 0 - 100.

 An activity can receive a start, stop, on, off, or value message:
   <activity>:0     (eg. tv:0)
   <activity>:100   (was "on", eg. "Booth Lamp:On")
   <activity>:value (eg. KitchenTemp:85


 http://www.github.com/rlisle/rv-arduino-libraries

 Written by Ron Lisle, ron@lisles.net

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2016-12-09: Convert to use value instead of isStarted
 2016-07-07: Remove dimming mode
 2016-01-17: Initial version
 ******************************************************************/

#ifndef activity_h
#define activity_h

#include "application.h"  // Defines String

class Activity
{
private:

public:
  String    name;
  int       value; // Depends on what it is: temperature or 0 = off, 100 = 0n, etc.

  Activity(String name, int value);
};

#endif /* activity_h */
