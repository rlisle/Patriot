/******************************************************************
 Patriot Relay control

 Features:
 - On/Off control of relays and other devices connected to GPIO.
 - Duration can be specified allowing automatic timed turn-off.

 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Changelog:
 2017-10-31: Initial creation
 ******************************************************************/

#pragma once

#include "Particle.h"
#include "device.h"

class Relay : public Device
{
 private:
    int8_t  _pinNum;
    int8_t  _duration;              // # seconds to keep on. 0 = forever
    long    _stopMillis;            // time in millis to auto-stop

 public:
    Relay(int8_t pinNum, String name, int8_t duration = 0);

    void    setPercent(int percent);

    void    loop();
};
