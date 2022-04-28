/******************************************************************
PatriotPIR plugin

Features:

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

******************************************************************/
#pragma once

#include "Particle.h"
#include <device.h>

#define TURNOFF_DELAY 2000

class PIR : public Device
{
private:
    int        _pin;
    long       _lastPollTime;
    long       _lastMotion;
    long       _timeoutMSecs;
    int        _filterValue;

    bool      isTimeToCheckSensor();
    bool      didSensorChange();
    bool      filterChanges(int newValue);
    void      notify();
    
public:
    PIR(int pinNum, String name, String room, long timeoutMSecs);

    void begin();
    void loop();
    
    // Override to prevent MQTT from setting _percent.
    void setValue(int percent) { return; };
};
