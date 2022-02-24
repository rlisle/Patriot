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
    int        _s1pin;                // Use Rx/Tx if both 0
    int        _s2pin;                // "
    
    int        _s1value;
    int        _s2value;
    
    long       _lastPollTime;

    bool      usingS1S2();
    bool      isTimeToCheckSensor();
    bool      didSensorChange();
    bool      didS1S2sensorChange();
    bool      didTxRxSensorChange();
    int       situation_judgment(int ad1, int ad2, int ad3, int ad4, int ad5);
    void      notify();
    
public:
    MR24(int s1pin, int s2pin, String name, String room);

    void begin();
    void loop();

    String status;    // String representation of last received data packet
    
    // Override to prevent MQTT from setting _percent.
    void setValue(int percent) { return; };
};
