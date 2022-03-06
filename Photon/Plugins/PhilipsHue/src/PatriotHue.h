/******************************************************************
PatriotHue plugin

Features:
- Read and set Philips Hue light value

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

******************************************************************/
#pragma once

#include "Particle.h"
#include <device.h>

class PatriotHue : public Device
{
private:
    long       _lastPollTime;

    bool      isTimeToCheckState();
    bool      didStateChange();
    void      notify();
    
public:
    PatriotHue(String name);

    void begin();
    void loop();
    
    void setValue(int percent);
};
