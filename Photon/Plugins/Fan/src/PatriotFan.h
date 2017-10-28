/******************************************************************
fan control

This object extends Patriot to support fan control.

IoT Behaviors are used in conjunction with a temperature device such
as DHT to define on/off and temperature control points.

TODO: provide interpolation between temperature set points.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2017-05-20: Convert to Patriot plugin library
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle library
2016-12-08: Convert to IoT model.
2016-07-06: Initial version
******************************************************************/
#pragma once

#include "Particle.h"
#include "device.h"

class Fan : public Device
{
 public:
    Fan(int pinNum, String name);

    virtual void    setPercent(int percent);

 private:
    int       calculateFanSpeed(int temperature);
    void      setFanSpeed(int percent);

    int       _pinNum;
    int       _percent;
};
