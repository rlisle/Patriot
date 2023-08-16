/******************************************************************
 NCD 16-Channel Dimmer control

 Features:
 - 0-4095 dimming control
 - Supports multiple boards ???
 
 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets: https://store.ncd.io/product/pca9685-16-channel-12-bit-5-amp-high-current-n-channel-pwm-pulse-width-modulation-light-dimmer-with-iot-interface/

 ******************************************************************/

#pragma once

#include "Particle.h"
#include <device.h>

class NCD16Dimmer : public Device
{
 private:
    int8_t  _lightNum;                 // Up to 16 lights supported, 0 based
    int8_t  _address;                  // Address of board (eg. 0x40)

    // Note: Photon didn't support Floating Point but Photon 2 does.
    // So maybe instead of using int (signed 32 bit) fixed point, we can use FP.
    // TODO: convert to FP - don't need the following (I think)
    // (Outputs are 12 bit unsigned, so Lower 19 bits truncated and sign discarded.)
    // (Using signed ints to simplify underflow (< 0))
    //
    int     _dimmingDuration;           // time in msecs
    int     _currentLevel;              // 0x00000000 to 0x7fffffff
    int     _targetLevel;               // transitioning to level
    int     _incrementPerMillisecond;   // Pre-calculated to minimize loop operations
    
    long    _lastUpdateTime;            // Time in msecs

    int8_t  initializeBoard();
    void    outputPWM();
    void    startSmoothDimming();
    int     convertPercent(int percent); // Convert 0-100 percent to 32 bit signed (0-7fffffff)

 public:
    NCD16Dimmer(int8_t address, int8_t lightNum, String name, String room, int8_t duration = 0);
    void    begin();
    void    reset();
    void    setValue(int percent);  // 0-100
    void    loop();
};
