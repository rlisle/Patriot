/******************************************************************
PatriotMR24 plugin

Features:
-

http://www.github.com/rlisle/Patriot

This is a Patriot plugin. After making changes use "particle library upload", etc.
 
Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

******************************************************************/

#include "PatriotMR24.h"
#include "IoT.h"

#define POLL_INTERVAL_MILLIS 100

/**
 * Constructor
 * @param pinNum int pin number that is connected to the sensor output
 * @param name  String name of the event to send when sensor changes
 */
MR24::MR24(int pinNum, String name, String room)
        : Device(name, room),
        _pin(pinNum)
{
    _type  = 'M';
    _value = 0;
}

void MR24::begin() {
    pinMode(_pin, INPUT);
    _lastPollTime = millis();
}

/**
 * loop()
 * This is called repeatedly to allow device to do its work.
 */
void MR24::loop()
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
bool MR24::isTimeToCheckSensor()
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
bool MR24::didSensorChange()
{
    int pinState = digitalRead(_pin);
    int newValue = pinState ? 100 : 0;
    if(newValue != _value) {
        _value = newValue;
        return true;
    }
    
    return false;
}


/**
 * notify()
 * Publish switch state
 */
void MR24::notify()
{
    String topic = "patriot/" + _name;
    String message = String(_value);
    IoT::mqttPublish(topic,message);
}
