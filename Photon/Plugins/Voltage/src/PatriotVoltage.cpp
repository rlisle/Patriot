/**
 Voltage

 Features:
 - Read and broadcast voltage on a pin

 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2022-12-12: Initial creation
 */

#include "Voltage.h"
#include "IoT.h"

#define POLL_INTERVAL_MILLIS 5000

/**
 * Constructor
 * @param boardAddress is the board address set by jumpers (0-7) 0x20-0x27
 * @param switchIndex is the switch number on the NCD board (0-3)
 * @param name String name used to address the relay.
 */
Voltage(int pinNum, String name, String room)
    : Device(name, room)
{
    _pinNum = pinNum;
    _lastPollTime = 0;
    _type         = 'V';
}

void Voltage::begin()
{
    // The pin defaults to analog input so need to do anything with it
    Log.info(name + " started on pin " + String(_pinNum));
}

/**
 * loop()
 */
void Voltage::loop()
{
    if (isTimeToReadVoltage())
    {
        if (didVoltageChange())
        {
            notify();
        }
    }
}

// Private Helper Methods
/**
 * isTimeToReadVoltage()
 * @return bool true if enough time has elapsed to sample input pin again
 */
bool Voltage::isTimeToReadVoltage()
{
    if(millis() > _lastPollTime + POLL_INTERVAL_MILLIS) {
        _lastPollTime = millis();
        return true;
    }
    return false;
}

/**
 * didVoltageChange()
 * @return bool true if voltage has changed since last reading
 */
bool Voltage::didVoltageChange()
{
    int newValue = analogRead(_pinNum);
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
void Voltage::notify()
{
    String message = String(_value);
    IoT::publishMQTT(_name + "/get/value",message);
}

