/******************************************************************
Device Names

This class represents all the devices on a controller.
These can be Lights, Fans, etc.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2020-11-22: Simplify using Devices object.
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle lib
2016-12-20: Initial version
******************************************************************/
#include "devicenames.h"
#include "constants.h"

String globalDevicesVariable;

DeviceNames::DeviceNames() {
    expose();
}

void DeviceNames::expose()
{
    if(!Particle.variable(kDevicesVariableName, globalDevicesVariable))
    {
        Serial.println("Error: Unable to expose " + kDevicesVariableName + " variable");
    }
}

void DeviceNames::buildDevicesVariable(Devices *devices)
{
    String newVariable = "";
    
    for(int i=0; i<devices->NumDevices();)
    {
        Device *device = newVariable += devices->getDeviceByNum(i);
        newVariable += device->name();
        if (i < _numDevices-1) {
            newVariable += ", ";
        }
    }
    if(newVariable.length() < kMaxVariableStringLength) {
        globalDevicesVariable = newVariable;
    } else {
        Serial.println("Devices variable is too long. Need to extend to a 2nd variable");
    }
}
