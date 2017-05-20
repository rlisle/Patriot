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
{
    _name          = name;
    _pinNum        = pinNum;
    _percent       = 0;
    pinMode(pinNum, OUTPUT);
    setFanSpeed(0);
}

/**
 * name
 * @return String name of fan
 */
String Fan::name() {
    return _name;
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
 * Get percent
 * @return Int current 0-100 percent value (0 stopped, 100 fastest)
 */
int Fan::getPercent()
{
    return _percent;
}

// Handle 'On', 'Off', 'Fast', 'Slow', '+', '-', or 0-100
/**
 * Convert commands such as "On", "Off", "Fast", "Slow" to a percent
 * @param speed String command name specific to a fan
 * @return Int mapped percent value (0 to 100)
 */
int Fan::convertCommandToPercent(String speed) {

    Serial.print("Set fan ");
    Serial.print(_name);
    Serial.print(" to ");
    Serial.println(speed);

    int percent = 0;
    if(speed.equalsIgnoreCase("on")) {
        percent = 100;

    } else if(speed.equalsIgnoreCase("off")) {
        percent = 0;

    } else if(speed == "+") {
        int increasedPercent = _percent + 10;
        if(increasedPercent > 100) increasedPercent = 100;
        percent = increasedPercent;

    } else if(speed == "-") {
        int decreasedPercent = _percent - 10;
        if(decreasedPercent < 0) decreasedPercent = 0;
        percent = decreasedPercent;

    } else {
        int newPercent = speed.toInt();
        if(newPercent > 100) newPercent = 100;
        percent = newPercent;
    }
    return percent;
}

void Fan::setOn()
{
    setPercent(100);
}

void Fan::setOff()
{
    setPercent(0);
}

bool Fan::isOn()
{
    return(_percent != 0);
}

bool Fan::isOff()
{
    return(_percent == 0);
}

void Fan::loop()
{
    // Nothing to do
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
