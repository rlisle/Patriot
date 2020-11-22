/******************************************************************
Devices

This class represents a collection of devices.
These can be Lights, Fans, etc.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2020-11-22: Integrate DeviceNames
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle lib
2016-07-24: Initial version
2016-09-09: Add behaviors
******************************************************************/
#pragma once

#include "Particle.h"
#include "device.h"

class Device;

class Devices
{
public:

  Devices();

  void        loop();
  void        addDevice(Device *device);
  Device*     getDeviceByNum(int deviceNum);
  Device*     getDeviceWithName(String name);
  int         numDevices();

  void        stateDidChange(States *states);

private:
  Device*     _devices;     // Linked list of device objects

  bool        _isVariableExposed;
  String      *_ptrToVariable;

  void buildDevicesVariable();
    void expose();
};
