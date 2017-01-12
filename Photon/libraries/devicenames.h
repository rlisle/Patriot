/******************************************************************
 Device Names

 This class represents all the devices in a controller.
 These can be Lights, Fans, etc.
 It exposes this list for use by other apps.
 This allows changing or extending the IoT network without
 requiring changes to all the apps.

 http://www.github.com/rlisle/rv-arduino-libraries

 Written by Ron Lisle, ron@lisles.net

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2017-01-10: Initial version
 ******************************************************************/

#ifndef devicenames_h
#define devicenames_h

#include "application.h"  // Defines String

#define MAX_NUM_DEVICENAMES 16

class DeviceNames
{
public:

  DeviceNames();

  int       addDevice(String device);
  bool      doesNameExist(String name);

private:
  uint8_t   _numDevices;
  String    _devices[MAX_NUM_DEVICENAMES];
  
  void      expose();
  void      buildDevicesVariable();
};

#endif /* deviceNamnes_h */
