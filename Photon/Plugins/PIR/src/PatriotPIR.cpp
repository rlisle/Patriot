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

#include "PatriotPIR.h"
#include "IoT.h"

#define POLL_INTERVAL_MILLIS 500

/**
 * Constructor
 * @param pinNum int pin number that is connected to the sensor output
 * @param name  String name of the event to send when sensor changes
 */
PIR::PIR(int pinNum, String name, String room, long timeoutMSecs)
        : Device(name, room),
        _pin(pinNum),
        _timeoutMSecs(timeoutMSecs)
{
    _type  = 'M';
    _value = 0;
    _filterValue = 0;
}

void PIR::begin() {
    pinMode(_pin, INPUT);
    _lastPollTime = millis();
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
    int pinState = digitalRead(_pin);
    int newValue = pinState ? 100 : 0;
    return filterChanges(newValue);
}

// Delay turn off until no motion for timeoutMSecs
bool PIR::filterChanges(int newValue) {
    // Filter the value
    if(newValue > 0) {
        _filterValue += 25;
        if(_filterValue > 100) {
            _filterValue = 100;
        }
    } else {
        _filterValue -= 25;
        if(_filterValue < 0) {
            _filterValue = 0;
        }
    }
    Log.info("PIR %d, filter: %d",newValue,_filterValue);
    
    if(_filterValue == 100) {
        _lastMotion = millis();
        if(_value < 100) {  // Was it off previously
            _value = 100;
            Log.info("PIR turning on");
            return true;
        }

    } else if(_filterValue == 0 && _value != 0){
        // Turning off?
        if(_lastMotion + _timeoutMSecs < millis()) {
            _value = 0;
            Log.info("PIR turning off");
            return true;
        }
    }
    return false;
}


/**
 * notify()
 * Publish switch state
 */
void PIR::notify()
{
    String topic = "patriot/" + _name;
    String message = String(_value);
    IoT::mqttPublish(topic,message);
}
