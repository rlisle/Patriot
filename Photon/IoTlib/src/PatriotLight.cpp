/**
 light LED dimming control

 Features:
 - Smooth dimming with duration

 Leverage floating point since Photon2 has hardware FP support.
 
 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 */

#include "IoT.h"
#include "math.h"

/**
 * Constructor
 * @param pinNum is the pin number that is connected to the light.
 * @param name String name used to address the light.
 * @param isInverted True if On = output LOW
 * @param forceDigital True if output On/Off only (even if pin supports PWM)
 */
Light::Light(int pinNum, String name, String room, int durationMSecs, int curve)
        : Device(name, room),
          _pin(pinNum),
          _dimmingMSecs(durationMSecs),
          _curve(curve)
{
    _targetPercent      = 0.0;
    _currentPercent     = 0.0;
    _incrementPerMSec   = 0.0;
    _lastUpdateTime     = 0;
    _type               = 'L';
}

void Light::begin() {
    pinMode(_pin, OUTPUT);
    if(isPwmSupported(_pin) == true) {
        _pinResolution = analogWriteResolution(_pin, 12);
        _maxLevel = (1 << _pinResolution) - 1;
    } else {
        _dimmingMSecs = 0;
    }
    outputPWM(0.0);
}

/**
 * Set value
 * @param value Int 0 to 100
 */
void Light::setValue(int value) 
{
    _targetPercent = value;
    if(_value == value) {
        outputPWM((float)value);
        return;
    }
    if(_dimmingMSecs == 0) {
        _value = value;
        outputPWM((float)_value);
    } else {
        startSmoothDimming();
    }
}

/**
 * Start smooth dimming
 * Use floatValue to smoothly transition
 */
void Light::startSmoothDimming() {
    _lastUpdateTime = millis();             // Starting now
    _currentPercent = (float)_value;        // Starting point
    float delta = (float)_targetPercent - _currentPercent;
    _incrementPerMSec = delta / (float)_dimmingMSecs;
}

/**
 * Private Methods
 */

/**
 * loop()
 * Called periodically to perform dimming, polling, etc.
 */
void Light::loop()
{
    // Is any fading transition done?
    if(_value == _targetPercent) {
        _currentPercent = (float)_targetPercent;
        return;
    }

    long loopTime = millis();
    float millisSinceLastUpdate = (loopTime - _lastUpdateTime);
    _currentPercent += _incrementPerMSec * millisSinceLastUpdate;
    _value = (int)round(_currentPercent);
    if(_incrementPerMSec > 0) {
        if(_currentPercent > _targetPercent) {
            _value = _targetPercent;
            _currentPercent = _targetPercent;
        }
    } else {
        if(_currentPercent < _targetPercent) {
            _value = _targetPercent;
            _currentPercent = _targetPercent;
        }
    }
    _lastUpdateTime = loopTime;
    outputPWM(_currentPercent);
};

/**
 * Set the output PWM value (0-maxLevel) based on 0-100 value
 */
void Light::outputPWM(float percent) {
    if(isPwmSupported(_pin)) {
        int pwm = convertToPinResolution(_currentPercent);
        analogWrite(_pin, pwm);
    } else {
        bool isOn = percent >= 50.0;
        digitalWrite(_pin, isOn ? HIGH : LOW);
    }
}

/**
 * Convert 0-100 to 0-_maxLevel exponential scale
 * 0 = 0, 100 = _maxLevel
 * This function is reusable by other dimming controllers
 * Dependencies: float _maxLevel, int _curve
 */
int Light::convertToPinResolution(float percent) {
    if(percent < 0.5) return 0;
    if(percent >= 99.5) return _maxLevel;
    float base = pow(_maxLevel, 1.0 / 100.0);
    float exponentialValue = pow(base, percent);
    float linearValue = percent * _maxLevel / 100.0;

    // Curve argument specifies curve shape
    // 0 = Linear
    // 1 = Exponential
    // 2 = 50/50 split
    // 3 = 75/25 split
    if(_curve == 0) {           // Linear
        return linearValue;
    } else if(_curve == 1) {    // Exponential (redundant)
        return exponentialValue;
    }
    // Else return 2 = 1/2 + 1/2, 3 = 1/3 + 2/3, 4 = 1/4 + 3/4
    float exponentialAmount = 1.0 / _curve;
    float linearAmount = (float)(_curve - 1) / (float)_curve;
    float linearPart = linearValue * linearAmount;
    float exponentialPart = exponentialValue * exponentialAmount;
    float combinedValue = constrain(exponentialPart + linearPart, 0.0, _maxLevel);
    return int(combinedValue);
}

/**
 * Is PWM pin?
 * @param pin number
 * @return bool true if pin supports PWM
 */
bool Light::isPwmSupported(int pin)
{
    switch(pin) {
        case D1:
        case D13:
        case D14:
        case D15:
        case D16:
            return TRUE;
        default:
            break;
    };
    return FALSE;
}
