/******************************************************************
 Smartthings light dimming control

 Features:
 - On/Off control
 - Smooth dimming with duration

 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2018-10-15: Initial version
 ******************************************************************/

#pragma once

#include "Particle.h"
#include "device.h"

class STLight : public Device
{
 private:
    String    _stname;                  // Smartthings device name
    int       _dimmingPercent;
    float     _dimmingDuration;
    float     _currentPercent;
    int       _targetPercent;
    int       _commandPercent;
    float     _incrementPerMillisecond;
    long      _lastUpdateTime;
    bool      _forceDigital;            // On/Off only

    bool      isAlreadyOn();
    bool      isAlreadyOff();
    void      changePercent(int percent);
    void      startSmoothDimming();

 public:
    STLight(String stname, String name, bool forceDigital=false);

    void      setPercent(int percent);
    int       getPercent();
    void      setOn();
    void      setOff();
    bool      isOn();
    bool      isOff();

    void      setDimmingPercent(int percent);
    void      setDimmingDuration(float duration);
    int       getDimmingPercent();
    float     getDimmingDuration();

    void      loop();
};
