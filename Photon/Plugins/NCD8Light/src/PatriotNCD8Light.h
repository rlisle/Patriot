/******************************************************************
 NCD Light control

 Features:
 - 0-4096 dimming control
 - Supports multiple boards
 
 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2020-02-11: Initial creation from modified NCD8Relay
 ******************************************************************/

#pragma once

#include "Particle.h"
#include "device.h"

class NCD8Light : public Device
{
 private:
    int8_t  _lightNum;                 // Up to 8 lights supported
    int8_t  _duration;                 // # seconds to transition. 0 = immediate

    static int8_t _currentState;
    int8_t   _address;            // Address of board (eg. 0x40)

    int      _targetPercent;
    float    _dimmingDuration;
    float    _currentPercent;          // Use for smooth dimming transitions
    float    _incrementPerMillisecond;
    long     _lastUpdateTime;

    int8_t  initializeBoard();
    void    outputPWM();
    void    startSmoothDimming();

 public:
    NCD8Light(int8_t address, int8_t lightNum, String name, int8_t duration = 0);
    void    setPercent(int percent);
    void    loop();
};
