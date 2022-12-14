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

#include "PatriotVoltage.h"
#include "IoT.h"

#define POLL_INTERVAL_MILLIS 5000

/**
 * Constructor
 * @param pinNum is the input pin (A0-A7)
 * @param name is the name used to address the value
 * @param room is the room where it is located
 * @param fullScale is the voltage value corresponding to 3.3v at the input
 * @param fixPoint is the fixed-point multiplier
 * So for example with a 10/1 resistor ladder, 3.3v at the input pin = 33 volts.
 */
Voltage::Voltage(int pinNum, String name, String room, float fullScale, int fixedPoint)
    : Device(name, room)
{
    _pinNum = pinNum;
    // Since Patriot uses integer values, assume fixed point 10x values (eg. 0.0 to 25.5 v)
    _stepValue = fullScale / 4095.0;
    _fixedPoint = float(fixedPoint);
    _lastPollTime = 0;
    _type         = 'V';
}

void Voltage::begin()
{
    // The pin defaults to analog input so need to do anything with it
    Log.info(_name + " started on pin " + String(_pinNum));
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
 * Value is scaled and clamped at 255
 */
bool Voltage::didVoltageChange()
{
    int readValue = analogRead(_pinNum);
    float floatValue = float(readValue) * _stepValue;
    int newValue = int(floatValue * _fixedPoint);
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
    
    IoT::publishMQTT(_name, message);
}

