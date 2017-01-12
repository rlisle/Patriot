/******************************************************************
 fan control

 This object controls fan operation.
 Currently, lowTemp and highTemp are used to set fan speed.
 Ultimately, behaviors will be used to define on/off and
 temperature control points, with interpolation between them.

 Features:
 - On/Off control
 - Temperature controlled PWM

 http://www.github.com/rlisle/rv-arduino-libraries

 Written by Ron Lisle, ron@lisles.net

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2016-12-08: Convert to IoT model.
 2016-07-06: Initial version
 ******************************************************************/
#ifndef fan_h
#define fan_h

#include "device.h"
#include "behaviors.h"

class Fan : public Device
{
public:
    Fan(String name, int pinNum);

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

    virtual int     addBehavior(Behavior* behavior);
    virtual void    performActivities(Activities* activities);

private:
    int       calculateFanSpeed(int temperature);
    void      setFanSpeed(int percent);

    Behaviors *_behaviors;
    String    _name;
    int       _pinNum;
    int       _percent;
    int       _manualPercent;   // Value set by voice command, etc.
};

#endif /* fan_h */
