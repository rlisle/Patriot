/**
 light LED dimming control

 Features:
 - On/Off control
 - Smooth dimming with duration

 http://www.github.com/rlisle/Patriot

 8.1.0 Converted to use 12 bits
 10.0.0 Photon 2
 
 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 */

#pragma once

#include "Particle.h"
#include <device.h>

class Light : public Device
{
 private:
    int       _pin;

    int       _targetValue;             // Same as _value?
    float     _dimmingDuration;
    float     _currentValue;          // Use for smooth dimming transitions
    float     _incrementPerMillisecond;

    long      _lastUpdateTime;
    bool      _isInverted;              // On state = LOW instead of default HIGH
    bool      _forceDigital;            // On/Off only, even if PWM supported

    void      startSmoothDimming();
    void      outputPWM();
    int       scalePWM(int percent);
    bool      isPwmSupported();

 public:
    Light(int pin, String name, String room, bool isInverted=false, bool forceDigital=false);
    void      begin();
    
    void      setValue(int value);      // Set light immediately

    void      setDimmingDuration(float duration);
    float     getDimmingDuration();

    void      loop();
};
