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

#include "PatriotLight.h"
#include "math.h"

/**
 * Constructor
 * @param pinNum is the pin number that is connected to the light.
 * @param name String name used to address the light.
 * @param isInverted True if On = output LOW
 * @param forceDigital True if output On/Off only (even if pin supports PWM)
 */
Light::Light(int pinNum, String name, bool isInverted, bool forceDigital)
        : Device(name, DeviceType::Light),
          _pin(pinNum),
          _isInverted(isInverted),
          _forceDigital(forceDigital)
{
    _dimmingDuration          = isPwmSupported() ? 2.0 : 0;
    _targetPercent            = 0;
    _currentPercent           = 0.0;
    _incrementPerMillisecond  = 0.0;
    _lastUpdateTime           = 0;
    pinMode(pinNum, OUTPUT);
    outputPWM();                        // Set initial state to persisted value
}

/**
 * Set percent
 * @param percent Int 0 to 100
 */
void Light::setPercent(int percent) {
    Log.info("Light " + _name + " setPercent: " + String(percent));
    changePercent(percent);
}

/**
 * Change percent
 * @param percent Int new percent value
 */
void Light::changePercent(int percent) {
    if(_targetPercent == percent) return;

    _targetPercent = percent;
    if(_dimmingDuration == 0.0 || isPwmSupported() == false) {
        _percent = percent;
        outputPWM();

    } else {
        startSmoothDimming();
    }
}

/**
 * Start smooth dimming
 * Use float _currentPercent value to smoothly transition
 * An alternative approach would be to calculate # msecs per step
 */
void Light::startSmoothDimming() {
    if((int)_percent == _targetPercent){
        Log.info("Light " + _name + " startSmoothDimming equal");
        return;
    }
    _currentPercent = _percent;
    _lastUpdateTime = millis();
    float delta = _targetPercent - _percent;
    _incrementPerMillisecond = delta / (_dimmingDuration * 1000);
    Log.info("Light " + _name + " startSmoothDimming target: " + String(_percent) + ", increment: " + String(_incrementPerMillisecond));
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
    if(_percent == _targetPercent) {
        return;
    }

    long loopTime = millis();
    float millisSinceLastUpdate = (loopTime - _lastUpdateTime);
    _currentPercent += _incrementPerMillisecond * millisSinceLastUpdate;
    _percent = _currentPercent;
    if(_incrementPerMillisecond > 0) {
        if(_currentPercent > _targetPercent) {
            _percent = _targetPercent;
            Log.info("Light "+_name+" loop: up done");
        }
    } else {
        if(_currentPercent < _targetPercent) {
            _percent = _targetPercent;
            Log.info("Light "+_name+" loop: down done");
        }
    }
    _lastUpdateTime = loopTime;
    outputPWM();
};

/**
 * Set the output PWM value (0-255) based on 0-100 percent value
 */
void Light::outputPWM() {
    if(isPwmSupported()) {
        int pwm = scalePWM(_percent);
        analogWrite(_pin, pwm);
    } else {
        bool isOn = _percent > 49;
        bool isHigh = (isOn && !_isInverted) || (!isOn && _isInverted);
        digitalWrite(_pin, isHigh ? HIGH : LOW);
    }
}

/**
 * Convert 0-100 percent to 0-255 exponential scale
 * 0 = 0, 100 = 255
 */
int Light::scalePWM(int percent) {
    if (percent == 0) return 0;
    if (percent >= 100) return 255;
    
    float base = 1.05697667;
    float pwm = pow(base,percent);
    if (pwm > 255) {
        return(255);
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
        case D0:
        case D1:
        case D2:
        case D3:
        case A4:
        case A5:
        case A7:    // aka WKP
        case RX:
        case TX:
            return _forceDigital ? FALSE : TRUE;
        default:
            break;
    };
    return FALSE;
}
