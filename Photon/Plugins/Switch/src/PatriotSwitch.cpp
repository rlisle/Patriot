/******************************************************************
PatriotSwitch plugin

Features:
- Read and debounce switch input

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

******************************************************************/

#include "PatriotSwitch.h"
#include "IoT.h"

#define POLL_INTERVAL_MILLIS 100
#define FILTER_INCREMENT 25

/**
 * Constructor
 * @param pinNum int pin number that is connected to the switch
 * @param name  String name of the event to send when switch changes
 */
Switch::Switch(int pinNum, String name)
        : Device(name),
        _pin(pinNum)
{
    _value = 0;
    _type  = 'S';
    _filter = 0;
}

void Switch::begin() {
    pinMode(_pin, INPUT_PULLUP);
    _lastPollTime = millis();
}

/**
 * loop()
 * This is called repeatedly to allow device to do its work.
 */
void Switch::loop()
{
    if (isTimeToCheckSwitch())
    {
        if (didSwitchChange())
        {
            notify();
        }
    }
}


// Private Helper Methods
/**
 * isTimeToCheckSwitch()
 * @return bool if enough time has elapsed to sample switch again
 */
bool Switch::isTimeToCheckSwitch()
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
 * didSwitchChange()
 *
 * Use a filter algorithm with hysteresis.
 * Increment/decrement if switch on/off
 * Only toggle switch state at 0 or 100
 *
 * @return bool if switch has changed since last reading
 */
bool Switch::didSwitchChange()
{
    int pinState = digitalRead(_pin);   // Inverted: 0 is on, 1 is off
    bool oldState = (_value != 0);

    if(pinState == 0) { // Is switch on?
        _filter += FILTER_INCREMENT;
        if(_filter > 100) {
            _filter = 100;
        }
    } else {
        _filter -= FILTER_INCREMENT;
        if(_filter < 0) {
            _filter = 0;
        }
    }
    
    if(oldState == false && _filter == 100) {
        _value = 100;
        return true;
    }
    
    if(oldState == true && _filter == 0) {
        _value = 0;
        return true;
    }
    
    return false;
}


/**
 * notify()
 * Publish switch state
 */
void Switch::notify()
{
    String topic = "patriot/" + _name;
    String message = String(_value);
    IoT::mqttPublish(topic,message);
}
