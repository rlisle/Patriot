/******************************************************************
PatriotMR24 plugin

Features:

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

******************************************************************/
#pragma once

#include "Particle.h"
#include <device.h>

class MR24 : public Device
{
private:
    int        _pin;                // Either this or use Rx/Tx
    long       _lastPollTime;

    bool      isTimeToCheckSensor();
    bool      didSensorChange();
    void      notify();
    
public:
    MR24(int pinNum, String name, String room);

    void begin();
    void loop();
    
    // Override to prevent MQTT from setting _percent.
    void setValue(int percent) { return; };
};
