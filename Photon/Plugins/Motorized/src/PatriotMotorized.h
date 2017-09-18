/******************************************************************
motorized device control

This object extends Patriot to support motorized devices such as
 motorized vents or curtains. It is expected that devices will take
 a fixed amount of time to open or close. That duration is specified
 when the device is instantiated.

 For example, the roof vent on my RV takes about 15 seconds to go
 from closed to fully open. So the vent device will be instantiated
 as follows:

 Motorized *vent = Motorized(D3, 15, "Roof Vent")

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2017-09-17: Initial version based on fan plugin.
******************************************************************/
#pragma once

#include "Particle.h"
#include "device.h"

enum MotorizedState { inactive, opening, closing };

class Motorized : public Device
{
 public:
    Motorized(int openPinNum, int closePinNum, long milliseconds, String name);

    virtual String  name();
    virtual void    setPercent(int percent);
    virtual int     getPercent();
    virtual int     convertCommandToPercent(String command);

    //TODO: Move these convenience methods to the device class as non-virtual
    virtual void    setOn();
    virtual void    setOff();
    virtual bool    isOn();
    virtual bool    isOff();

    virtual void    loop();

 private:
    unsigned long neededDuration();
    void          turnOffMotor();

    String    _name;
    int       _openPinNum;
    int       _closePinNum;
    int       _percent;
    long      _milliseconds; // Milliseconds needed to fully open or close

    int       _state;        // 0=inactive, 1=opening, 2=closing TODO: switch to enum
    int       _startPercent; // Origin of change when opening/closing
    long      _startMillis;  // Time when change started
};
