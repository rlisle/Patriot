/******************************************************************
 light LED dimming control

 Features:
 - On/Off control
 - Smooth dimming with duration

 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2019-11-09: v3.1.0 Add local pin
 2019-01-03: v3.0.0 Assume use of Backup SRAM to persist percent across resets.
 2017-10-28: Convert to v2.
 2017-09-20: Add support for inverted and non-analog outputs
 2017-05-19: Extract to separate plugin library
 2017-05-15: Make devices generic
 2017-03-24: Rename Patriot
 2016-07-07: Remove dimming mode
 2016-01-17: Initial version
 ******************************************************************/

#pragma once

#include "Particle.h"
#include "device.h"

class Light : public Device
{
 private:
    int       _pin;

    int       _targetPercent;
    float     _dimmingDuration;
    float     _currentPercent;          // Use for smooth dimming transitions
    float     _incrementPerMillisecond;

    long      _lastUpdateTime;
    bool      _isInverted;              // On state = LOW instead of default HIGH
    bool      _forceDigital;            // On/Off only, even if PWM supported

    bool      isAlreadyOn();
    bool      isAlreadyOff();
    void      changePercent(int percent);
    void      startSmoothDimming();
    void      outputPWM();
    bool      isPwmSupported();

 public:
    Light(int pin, String name, bool isInverted=false, bool forceDigital=false);
    void      setPercent(int percent);      // Set light immediately
    void      setOn();                      // Same as setSwitch 100
    void      setOff();                     // Same as setSwitch 0
    bool      isOn();
    bool      isOff();

    void      setDimmingDuration(float duration);
    float     getDimmingDuration();

    void      loop();
};
