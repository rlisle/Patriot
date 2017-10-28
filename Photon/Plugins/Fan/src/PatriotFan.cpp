/******************************************************************
fan control

This object extends Patriot to support fan control.

IoT Behaviors are used in conjunction with a temperature device such
as DHT to define on/off and temperature control points.

TODO: provide interpolation between temperature set points.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2017-10-28: Convert to v2.
2017-05-20: Convert to Patriot plugin library
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle library
2016-12-08: Convert to IoT model.
2016-07-06: Initial version
******************************************************************/

#include "PatriotFan.h"

/**
 * Constructor
 * @param pinNum is the pin number that is connected to the fan.
 * @param name String name used to address the fan.
 */
Fan::Fan(int pinNum, String name)
    : Device(name)
{
    _pinNum        = pinNum;
    _percent       = 0;
    pinMode(pinNum, OUTPUT);
    setFanSpeed(0);
}

/**
 * Set percent
 * @param percent Int 0 to 100
 */
void Fan::setPercent(int percent)
{
    setFanSpeed(percent);
}

/**
 * Set fan speed
 * @param percent value (0-100)
 * The percent value 0-100 will be mapped to PWM range 0-255
 */
void Fan::setFanSpeed(int percent)
{
    _percent = percent;
    float pwm = _percent * 256.0 / 100.0;
    analogWrite(_pinNum, (int)pwm);
}
