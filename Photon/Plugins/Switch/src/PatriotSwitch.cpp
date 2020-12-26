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

#define MILLIS_PER_SECOND 1000
#define POLL_INTERVAL_MILLIS 100

/**
 * Constructor
 * @param pinNum int pin number that is connected to the switch
 * @param name  String name of the event to send when switch changes
 */
Switch::Switch(int pinNum, String name)
        : Device(name, DeviceType::Switch),
        _pin(pinNum)
{
    _percent = 0;
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
 * @return bool if switch has changed since last reading
 */
bool Switch::didSwitchChange()
{
    int pinState = digitalRead(_pin);
    bool newState = (pinState == 0);
    if (newState == isOn())
    {
        return false;
    }
    _percent = newState ? 100 : 0;
    return true;
}


/**
 * notify()
 * Publish switch state
 */
void Switch::notify()
{
    String topic = "patriot/" + _name;
    String message = String(_percent);
    publish(topic,message);
}
