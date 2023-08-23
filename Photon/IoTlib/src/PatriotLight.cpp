/**
 light LED dimming control

 Features:
 - Smooth dimming with duration

 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 */
// TODO: Convert to 12 bit

#include "IoT.h"
#include "math.h"

/**
 * Constructor
 * @param pinNum is the pin number that is connected to the light.
 * @param name String name used to address the light.
 * @param isInverted True if On = output LOW
 * @param forceDigital True if output On/Off only (even if pin supports PWM)
 */
Light::Light(int pinNum, String name, String room, bool isInverted, bool forceDigital)
        : Device(name, room),
          _pin(pinNum),
          _isInverted(isInverted),
          _forceDigital(forceDigital)
{
    _targetValue             = 0;
    _currentValue            = 0.0;
    _incrementPerMillisecond = 0.0;
    _lastUpdateTime          = 0;
    _type                    = 'L';
}

void Light::begin() {
    _dimmingDuration = isPwmSupported() ? 2.0 : 0;
    pinMode(_pin, OUTPUT);
    int res = analogWriteResolution(_pin,12);
    int maxFreq = analogWriteMaxFrequency(_pin);
    outputPWM();
}

/**
 * Set value
 * @param value Int 0 to 100
 */
void Light::setValue(int value) {
    if(_targetValue == value) {
        outputPWM();
        return;
    }

    _targetValue = value;
    if(_dimmingDuration == 0.0 || isPwmSupported() == false) {
        _value = value;
        outputPWM();

    } else {
        startSmoothDimming();
    }
}

/**
 * Start smooth dimming
 * Use float _currentValue to smoothly transition
 * An alternative approach would be to calculate # msecs per step
 */
void Light::startSmoothDimming() {
    if((int)_value == _targetValue){
        return;
    }
    _currentValue = _value;
    _lastUpdateTime = millis();
    float delta = _targetValue - _value;
    _incrementPerMillisecond = delta / (_dimmingDuration * 1000);
}

/**
 * Set dimming duration
 * This will only affect any future transitions
 * @param duration float number of seconds
 */
void Light::setDimmingDuration(float duration) {
    _dimmingDuration = duration;
}

/**
 * Get dimming duration
 * @return float number of dimming duration seconds
 */
float Light::getDimmingDuration() {
    return _dimmingDuration;
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
    // Is fading transition underway?
    if(_value == _targetValue) {
        return;
    }

    long loopTime = millis();
    float millisSinceLastUpdate = (loopTime - _lastUpdateTime);
    _currentValue += _incrementPerMillisecond * millisSinceLastUpdate;
    _value = _currentValue;
    if(_incrementPerMillisecond > 0) {
        if(_currentValue > _targetValue) {
            _value = _targetValue;
        }
    } else {
        if(_currentValue < _targetValue) {
            _value = _targetValue;
        }
    }
    _lastUpdateTime = loopTime;
    outputPWM();
};

/**
 * Set the output PWM value (0-255) based on 0-100 value
 */
void Light::outputPWM() {
    if(isPwmSupported()) {
        int pwm = scalePWM(_value);
        analogWrite(_pin, pwm);
    } else {
        bool isOn = _value > 49;
        bool isHigh = (isOn && !_isInverted) || (!isOn && _isInverted);
        digitalWrite(_pin, isHigh ? HIGH : LOW);
    }
}

/**
 * Convert 0-100 to 0-0x00ffffff (12 bit) exponential scale
 * 0 = 0, 100 = 0xffffff
 */
int Light::scalePWM(int value) {
    //TODO: This is too extreme. Adjust algorithm
    if (value == 0) return 0;
    if (value >= 100) return 0xffffff;
    
//    float base = 1.05697667;        // 255
    float base = 1.08673221;          // 4095
    float pwm = pow(base,value);
    if (pwm > 4095) {
        return(4095);
    }
    return (int) pwm;
}

/**
 * Is PWM pin?
 * @param pin number
 * @return bool true if pin supports PWM
 */
bool Light::isPwmSupported()
{
    switch(_pin) {
        case D1:
        case D15:
        case D17:
        case A2:
        case A5:
            return _forceDigital ? FALSE : TRUE;
        default:
            break;
    };
    return FALSE;
}
