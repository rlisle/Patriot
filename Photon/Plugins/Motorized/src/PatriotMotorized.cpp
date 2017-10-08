/******************************************************************
motorized device control

This object extends Patriot to support motorized devices such as
 motorized vents or curtains. It operates in 2 different modes.
 In the default mode, the open or close output is held for the
 desired percentage of open/close. In pulse mode the open or close
 pin is pulsed on for 1 second, then both are pulsed to stop movement.
 It is expected that devices will take a fixed amount of time to
 open or close. That duration is specified when the device is instantiated.

 For example, the roof vent on my RV takes about 5 seconds to go
 from closed to fully open. So the vent device will be instantiated
 as follows:

 Motorized *vent = Motorized(D1, D2, 5, "Roof Vent")

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2017-10-08: Add pulse mode
2017-09-17: Initial version based on fan plugin.
******************************************************************/

#include "PatriotMotorized.h"

/**
 * Constructor
 * @param openPinNum is the pin number that is connected to the open motor
 * @param closePinNum is the pin number that is connected to the close motor
 * @param duration is the # seconds to completely open/close
 * @param name String name used to address this device.
 */
Motorized::Motorized(int8_t openPinNum, int8_t closePinNum, int8_t duration, String name)
{
    _name               = name;
    _openPinNum         = openPinNum;
    _closePinNum        = closePinNum;
    _durationSeconds    = duration;     // # seconds to go from fully closed to open
    _pulseDurationMsecs = 0;            // Duration of pulse (used in pulse mode only)
    _mode               = 0;            // 1 = pulse mode, pulse both to stop
    _percent            = 0;            // 0 = closed, 100 = fully open
    _state              = 0;            // 0 = not changing

    _stopTimeMsecs      = 0;
    _pulseTimeMsecs     = 0;

    pinMode(openPinNum, OUTPUT);
    pinMode(closePinNum, OUTPUT);
    digitalWrite(openPinNum, LOW);
    digitalWrite(closePinNum, LOW);
}


Motorized::setPulseMode(int8_t pulseDurationMsecs)
{
    _mode = 1;          // Pulse either pin to start, pulse both to stop
    _pulseMsecs = 0;    // Used to end pulses
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

    _stopMsecs = calcStopTime(percent);

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

    if(_mode != 0)
    {
        _pulseMsecs = millis() + _pulseMsecs;
    }
}

void Motorized::loop()
{
    if(_state == 0) return;     // Nothing to do if we're idle

    // Has the motor been on long enough?
    if(_stopMsecs != 0)
    {
        if(millis() >= _stopMsecs)
        {
            turnOffMotor();
        }
    }

    // Has our pulse been on long enough?
    if(_pulseMsecs != 0)
    {
        if(millis() >= _pulseMsecs)
        {
            _pulseMsecs = 0;
            digitalWrite(_openPinNum, LOW);
            digitalWrite(_closePinNum, LOW);

            // Was this the stop pulse?
            if(_stopMsecs == 0)
            {
                _state = 0;
            }
        }
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

    if(_mode == 0)
    {
        _state = 0;     // All done
        digitalWrite(_openPinNum, LOW);
        digitalWrite(_closePinNum, LOW);

    } else {            // Pulse both pins to "stop"
        _pulseMsecs = millis() + _pulseDuration;
        digitalWrite(_openPinNum, HIGH);
        digitalWrite(_closePinNum, HIGH);
    }
    _stopMsecs = 0;
}
