/******************************************************************
PatriotSwitch plugin

Features:
- Read and debounce switch input

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

******************************************************************/
#pragma once

#include "Particle.h"
#include <device.h>

class Switch : public Device
{
private:
    int        _pin;
    long       _lastPollTime;

    bool      isTimeToCheckSwitch();
    bool      didSwitchChange();
    void      notify();
    
public:
    Switch(int pinNum, String name);

    void begin();
    void loop();
    
    // Override to prevent MQTT from setting _percent.
    void setValue(int percent) { return; };
};
