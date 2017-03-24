/******************************************************************
Devices

This class represents a collection of devices.
These can be Lights, Fans, etc.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle lib
2016-07-24: Initial version
2016-09-09: Add behaviors
******************************************************************/
#pragma once

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
