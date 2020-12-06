/******************************************************************
Devices

This class represents a collection of devices.
These can be Lights, Fans, etc.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

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
    
    void buildDevicesVariable();
    void expose();
    void log(String message, PLogLevel logLevel = LogDebug);
};
