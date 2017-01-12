/******************************************************************
 Device abstract class

 This virtual class provides a common header for all devices
 such as Lights, Switches, Fans, etc.

 http://www.github.com/rlisle/rv-arduino-libraries

 Written by Ron Lisle, ron@lisles.net

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2016-09-11: Initial version
 ******************************************************************/

#ifndef device_h
#define device_h

#include "application.h"  // Defines String
#include "behavior.h"

class Behavior;

class Device
{
public:
  virtual String name() = 0;
  virtual void   setPercent(int percent) = 0;
  virtual int    getPercent() = 0;

  // Each device may use different commands (eg. bright vs fast vs cold)
  // These need to be converted to a level by each device
  virtual int    convertCommandToPercent(String command) = 0;

  //TODO: Move these convenience methods to a device class as non-virtual
  virtual void   setOn() = 0;
  virtual void   setOff() = 0;
  virtual bool   isOn() = 0;
  virtual bool   isOff() = 0;

  virtual void   loop() = 0;

  virtual int    addBehavior(Behavior* behavior) = 0;
  virtual void   performActivities(Activities* activities) = 0;

};

#endif /* device_h */
