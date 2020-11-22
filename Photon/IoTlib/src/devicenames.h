/******************************************************************
Device Names

This class represents all the devices in a controller.
These can be Lights, Fans, etc.
It exposes this list for use by other apps.
This allows changing or extending the IoT network without
requiring changes to all the apps.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Datasheets:

Changelog:
2020-11-22: Simplify using devices object.
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle lib
2017-01-10: Initial version
******************************************************************/
#pragma once

#include "application.h"  // Defines String

class DeviceNames
{
public:
    
    DeviceNames();
    
    void      expose();
    void      buildDevicesVariable(Devices *devices);
};
