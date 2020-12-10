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
#include "device.h"

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

    void loop();
    
    bool shouldAutoCreateBehavior() { return false; };
    
    // Override to prevent MQTT from setting _percent.
    // Needed because of no auto behavior.
    // Our own generated MQTT message will reset _percent back to 0.
    void setPercent(int percent) { return; };
};
