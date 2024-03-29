/******************************************************************
PatriotPIR plugin

Features:
-

http://www.github.com/rlisle/Patriot

This is a Patriot plugin. After making changes use "particle library upload", etc.
 
Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

******************************************************************/

#include "IoT.h"

#define POLL_INTERVAL_MILLIS 500

/**
 * Constructor
 * @param pinNum int pin number that is connected to the sensor output
 * @param name  String name of the event to send when sensor changes
 * @param room String
 * @parm timeoutSecs #secs to keep on after motion stops (was msecs)
 */
PIR::PIR(int pinNum, String name, String room, int timeoutSecs, void (*handler)(int,int))
        : Device(name, room, 'M', handler),
        _pin(pinNum),
        _timeoutMSecs(timeoutSecs * 1000)
{
    _type  = 'M';
    _value = 0;
}

void PIR::begin() {
    pinMode(_pin, INPUT);
    _lastPollTime = millis();
    _lastMotion = 0 - _timeoutMSecs;
}

/**
 * loop()
 * This is called repeatedly to allow device to do its work.
 */
void PIR::loop()
{
    if (isTimeToCheckSensor())
    {
        if (didSensorChange())
        {
            notify();
        }
    }
}


// Private Helper Methods
/**
 * isTimeToCheckSensor()
 * @return bool if enough time has elapsed to sample switch again
 */
bool PIR::isTimeToCheckSensor()
{
    long currentTime = millis();
    if (currentTime < _lastPollTime + POLL_INTERVAL_MILLIS)
    {
        return false;
    }
    _lastPollTime = currentTime;
    return true;
}


/**
 * didSensorChange()
 *
 * @return bool if switch has changed since last reading
 */
bool PIR::didSensorChange()
{
    int prevValue = _value;
    _value = stateWithDelayedOff();
    return _value != prevValue;
}

/**
 * stateWithDelayedOff()
 *
 * @return bool state with delayed turn off
 */
int PIR::stateWithDelayedOff()
{
    int pinState = digitalRead(_pin);
    int newValue = pinState ? 100 : 0;
    
    if(newValue == 100) {
        _lastMotion = millis();
    
    // Turn off only after delay
    } else {
        if(_lastMotion + _timeoutMSecs > (long)millis()) {
            newValue = 100;
        }
    }
    return newValue;
}

/**
 * notify()
 * Publish switch state
 */
void PIR::notify()
{
    String message = String(_value);
    IoT::publishMQTT(_name,message);
}
