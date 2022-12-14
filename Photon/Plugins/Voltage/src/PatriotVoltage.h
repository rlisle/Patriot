/**
 Patriot Voltage Monitoring

 This plugin supports monitoring of the voltage on a Photon analog pin (A0-A7)
 
 Warning: the voltage applied to a pin must not exceed 3.3v or damage to the Photon may result.
 
 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2022-12-12: Initial creation
 */

#pragma once

#include "Particle.h"
#include <device.h>

class Voltage : public Device
{
 private:
    system_tick_t   _lastPollTime;
    int             _pinNum;
    float           _stepValue;
    float           _fixedPoint;

    bool    isTimeToReadVoltage();
    bool    didVoltageChange();
    void    notify();

 public:
    Voltage(int pinNum, String name, String room, float fullScale, int fixedPoint);
    
    void    begin();
    void    loop();
};
