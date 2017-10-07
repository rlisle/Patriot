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
Motorized::Motorized(int8_t openPinNum, int8_t closePinNum, int8_t duration, String name)
{
    _name             = name;
    _openPinNum       = openPinNum;
    _closePinNum      = closePinNum;
    _duration         = duration;
    _percent          = 0;          // 0 = closed, 100 = fully open
    _state            = 0;          // 0 = not changing
    _stopMillis       = 0;
    pinMode(openPinNum, OUTPUT);
    pinMode(closePinNum, OUTPUT);
    digitalWrite(openPinNum, LOW);
    digitalWrite(closePinNum, LOW);
}

/**
 * Set percent
 * This is the method used to start changing position
 *
 * @param percent Int 0 to 100
 */
void Motorized::setPercent(int percent)
{
    Serial.print("setPercent ");
    Serial.println(percent);

    if(percent == _percent) return;

    Serial.println("DEBUG: changing percent to "+String(percent));

    _stopMillis = calcStopTime(percent);

    if(percent > _percent) // Are we opening?
    {
        _state = 1;
        digitalWrite(_openPinNum, HIGH);
    }
    else
    {
        _state = 2;
        digitalWrite(_closePinNum, HIGH);
    }
    _percent = percent;             // Target percent
}

void Motorized::loop()
{
    // TODO: wait until elapsed time to shut off motor
    if(_state == 0 || _stopMillis == 0) return;

    // Has the motor been on long enough?
    if(millis() >= _stopMillis)
    {
        turnOffMotor();
    }
}

unsigned long Motorized::calcStopTime(int percent)
{
    int deltaPercent = percent - _percent;
    if(deltaPercent < 0) deltaPercent = -deltaPercent;
    unsigned long stopTime = millis();
    stopTime += (_duration * 100000) / deltaPercent;

    Serial.println("Current time = "+String(millis())+", stop time = "+String(stopTime));

    return stopTime;
}

void Motorized::turnOffMotor()
{
    Serial.println("Turning off motor");

    _stopMillis = 0;

    digitalWrite(_openPinNum, LOW);
    digitalWrite(_closePinNum, LOW);

    _state = 0;
}
