/******************************************************************
Device abstract class

This virtual class provides a common header for all devices
such as Lights, Switches, Fans, etc.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2017-05-15: Make devices generic
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle lib
2016-09-11: Initial version
******************************************************************/
#pragma once

#include "behavior.h"

class Behavior;

class Device {
public:
    // Required
    virtual String name() = 0;

    virtual int getPercent() = 0;

    virtual bool isOn() = 0;

    // Optional
    virtual void setPercent(int percent) {};

    // Each device may use different commands (eg. bright vs fast vs cold)
    // These need to be converted to a level by each device
    virtual int convertCommandToPercent(String command) { return 0; };

    //TODO: Move these convenience methods to a device class as non-virtual
    virtual void setOn() {};

    virtual void setOff() {};

    virtual bool isOff() { return isOn() == false; };

    virtual void loop() {};

};
