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

#include "PatriotRelay.h"

#define MILLIS_PER_SECOND 1000

/**
 * Constructor
 * @param pinNum is the GPIO pin
 * @param name String name used to address the device.
 * @param duration is an optional automatic turn off duration in seconds.
 */
Relay::Relay(int8_t pinNum, String name, int8_t duration)
    : Device(name)
{
    _pinNum     = pinNum;
    _duration   = duration;
    _stopMillis = 0;

    pinMode(pinNum, OUTPUT);
}

/**
 * Set percent
 * This is how things are turned on/off in Patriot
 * @param percent Int 0 to 100. 0 = off, >0 = on
 */
void Relay::setPercent(int percent)
{
    if(percent > 0)
    {
        digitalWrite(pinNum, HIGH);
        if(duration != 0)
        {
            _stopMillis = millis() + (_duration * MILLIS_PER_SECOND);
        }
    }
    else
    {
        digitalWrite(pinNum, LOW);
        _stopMillis = 0;
    }
    _percent = percent;
}

/**
 * Private Methods
 */

/**
 * loop()
 */
void Relay::loop()
{
    if(_stopMillis != 0)
    {
        if(millis() >= _stopMillis)
        {
            _stopMillis = 0;
            setPercent(0);
        }
    }
};
