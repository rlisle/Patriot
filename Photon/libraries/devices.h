/******************************************************************
 Devices

 This class represents a collection of devices.
 These can be Lights, Fans, etc.

 http://www.github.com/rlisle/rv-arduino-libraries

 Written by Ron Lisle, ron@lisles.net

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2016-07-24: Initial version
 2016-09-09: Add behaviors
 ******************************************************************/

#ifndef devices_h
#define devices_h

#include "device.h"

#define MAX_NUM_DEVICES 64

class Devices
{
public:

  Devices();

  void        loop();
  int         addDevice(Device *device);
  Device*     getDeviceByNum(int deviceNum);
  Device*     getDeviceWithName(String name);
  void        performActivities(Activities *activities);

  bool       expose(String variableName);

private:
  uint8_t     _numDevices;
  Device**    _devices;

  bool        _isVariableExposed;
  String      *_ptrToVariable;

  void buildDeviceVariable();
};

#endif /* devices_h */
