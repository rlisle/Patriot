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
    int        _s1pin;                // Either this or use Rx/Tx
    int        _s2pin;                // "
    long       _lastPollTime;

    bool      isTimeToCheckSensor();
    bool      didSensorChange();
//    int       situation_judgment(int ad1, int ad2, int ad3, int ad4, int ad5);
    void      notify();
    
public:
    MR24(int s1PinNum, int s2PinNum, String name, String room);

    void begin();
    void loop();
    
    // Override to prevent MQTT from setting _percent.
    void setValue(int percent) { return; };
};
