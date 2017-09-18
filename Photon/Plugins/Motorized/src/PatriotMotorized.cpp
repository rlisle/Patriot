/******************************************************************
motorized device control

This object extends Patriot to support motorized devices such as
 motorized vents or curtains. It is expected that devices will take
 a fixed amount of time to open or close. That duration is specified
 when the device is instantiated.

 For example, the roof vent on my RV takes about 15 seconds to go
 from closed to fully open. So the vent device will be instantiated
 as follows:

 Motorized *vent = Motorized(D3, 15, "Roof Vent")

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2017-09-17: Initial version based on fan plugin.
******************************************************************/

#include "PatriotMotorized.h"

/**
 * Constructor
 * @param pinNum is the pin number that is connected to the fan.
 * @param duration is the number of seconds required to open/close.
 * @param name String name used to address this device.
 */
Motorized::Motorized(int openPinNum, int closePinNum, int milliseconds, String name)
{
    _name             = name;
    _openPinNum       = openPinNum;
    _closePinNum      = closePinNum;
    _milliseconds     = milliseconds;
    _percent          = 0;          // 0 = closed, 100 = fully open
    _state            = 0;          // 0 = not changing
    pinMode(openPinNum, OUTPUT);
    pinMode(closePinNum, OUTPUT);
    digitalWrite(openPinNum, LOW);
    digitalWrite(closePinNum, LOW);
}

/**
 * name
 * @return String name of fan
 */
String Motorized::name() {
    return _name;
}

/**
 * Set percent
 * This is the method used to start changing position
 *
 * @param percent Int 0 to 100
 */
void Motorized::setPercent(int percent)
{
    if(percent == _percent) return;

    _startPercent = percent;    // May be unnecessary
    _percent = percent;         // Target percent
    _startMillis = millis();    // Remember starting time in milliseconds

    if(percent > _startPercent) // Are we opening?
    {
        _state = 1;
        digitalWrite(_openPin, HIGH);
    }
    else
    {
        _state = 2;
        digitalWrite(_closePin, HIGH);
    }
}

/**
 * Get percent
 * @return Int current 0-100 percent value (0 closed, 100 open)
 */
int Motorized::getPercent()
{
    return _percent;
}

// Handle command words like 'Open', 'Close', '+', '-', or 0-100
/**
 * Convert commands such as "Open", "Close", and "Half" to a percent
 * @param position String command name specific to a motorized device
 * @return Int mapped percent value (0 to 100)
 */
int Motorized::convertCommandToPercent(String position)
{
    Serial.print("Set motorized ");
    Serial.print(_name);
    Serial.print(" to ");
    Serial.println(position);

    int percent = 0;
    if(position.equalsIgnoreCase("open")) {
        percent = 100;

    } else if(position.equalsIgnoreCase("close")) {
        percent = 0;

    } else if(position.equalsIgnoreCase("closed")) {
        percent = 0;

    } else if(position == "+") {
        int increasedPercent = _percent + 10;
        if(increasedPercent > 100) increasedPercent = 100;
        percent = increasedPercent;

    } else if(position == "-") {
        int decreasedPercent = _percent - 10;
        if(decreasedPercent < 0) decreasedPercent = 0;
        percent = decreasedPercent;

    } else {
        int newPercent = position.toInt();
        if(newPercent > 100) newPercent = 100;
        percent = newPercent;
    }
    return percent;
}

void Motorized::setOn()
{
    setPercent(100);
}

void Motorized::setOff()
{
    setPercent(0);
}

bool Motorized::isOn()  // TODO: isOpen or isClosed would be more appropriate
{
    return(_percent != 0);
}

bool Motorized::isOff()
{
    return(_percent == 0);
}

void Motorized::loop()
{
    // TODO: wait until elapsed time to shut off motor
    if(_state == 0) return;

    // Has the motor been on long enough?
    if(millis() >= _startMillis + neededDuration())
    {
        turnOffMotor();
        _state = 0;
    }
}

long Motorized::neededDuration()
{
    int deltaPercent = _percent - _startPercent;
    if(deltaPercent < 0) deltaPercent = -deltaPercent;
    return _milliseconds * 100 / deltaPercent;
}

void Motorized::turnOffMotor()
{
    digitalWrite(_openPinNum, LOW);
    digitalWrite(_closePinNum, LOW);
}
