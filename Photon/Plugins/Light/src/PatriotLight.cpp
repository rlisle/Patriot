/******************************************************************
 light LED dimming control

 Features:
 - On/Off control
 - Smooth dimming with duration

 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2017-05-19: Extract to separate plugin library
 2017-05-15: Make devices generic
 2017-03-24: Rename Patriot
 2016-07-07: Remove dimming mode
 2016-01-17: Initial version
 ******************************************************************/

#include "PatriotLight.h"

/**
 * Constructor
 * @param pinNum is the pin number that is connected to the light.
 * @param name String name used to address the light.
 * @param isInverted True if On = output LOW
 * @param forceDigital True if output On/Off only (even if pin supports PWM)
 */
Light::Light(int pinNum, String name, bool isInverted, bool forceDigital) : Device(name), _pin(pinNum), _isInverted(isInverted), _forceDigital(forceDigital)
{
    Serial.println("Light constructor name: "+name+".");

    _dimmingPercent           = 100;                                // On full
    _dimmingDuration          = isPwmSupported() ? 2.0 : 0;
    _currentPercent           = 0.0;
    _targetPercent            = 0;
    _incrementPerMillisecond  = 0.0;
    _lastUpdateTime           = 0;
    _commandPercent            = 0;
    pinMode(pinNum, OUTPUT);
    outputPWM();                        // Set initial off state
    Serial.println("Light constructor returning _name: "+_name+".");
}


/**
 * convert commands such as "on" or "off" to percent
 * @param dimming String command name specific to this device
 * @return equivalent percent value
 */
int Light::convertCommandToPercent(String dimming) {

    Serial.println("Set light "+_name+" to "+String(dimming));

    int percent = 0;
    if(dimming.equalsIgnoreCase("on")) {
        percent = 100;

    } else if(dimming.equalsIgnoreCase("off")) {
        percent = 0;

    } else if(dimming.equalsIgnoreCase("toggle")) {
        percent = (isAlreadyOn() ? 0 : _dimmingPercent);

    } else if(dimming == "+") {
        int increasedPercent = _dimmingPercent + 10;
        if(increasedPercent > 100) increasedPercent = 100;
        percent = increasedPercent;

    } else if(dimming == "-") {
        int decreasedPercent = _dimmingPercent - 10;
        if(decreasedPercent < 0) decreasedPercent = 0;
        percent = decreasedPercent;

    } else {
        int newPercent = dimming.toInt();
        if(newPercent > 100) newPercent = 100;
        percent = newPercent;
    }
    return percent;
}

/**
 * Set percent
 * @param percent Int 0 to 100
 */
void Light::setPercent(int percent) {
    Particle.publish("DEBUG", "setPercent: "+String(percent), 60, PRIVATE);
    _commandPercent = percent;
    changePercent(percent);
}

/**
 * Get percent
 * @return Int current 0-100 percent value
 */
int Light::getPercent() {
    return _currentPercent;
}

/**
 * Set On
 */
void Light::setOn() {
    if(isAlreadyOn()) return;
    changePercent(_dimmingPercent);
}

/**
 * Change percent
 * @param percent Int new percent value
 */
void Light::changePercent(int percent) {
    Particle.publish("DEBUG", "changePercent: "+String(percent), 60, PRIVATE);
    if(_targetPercent == percent) return;

    _targetPercent = percent;
    if(_dimmingDuration == 0.0 || isPwmSupported() == false) {
        Particle.publish("DEBUG", "Light setting percent to: "+String(percent), 60, PRIVATE);
        _currentPercent = percent;
        outputPWM();

    } else {
        Particle.publish("DEBUG", "startSmoothDimming: "+String(percent), 60, PRIVATE);
        startSmoothDimming();
    }
}

/**
 * Is already on?
 * @return bool true if light is on
 */
bool Light::isAlreadyOn() {
    return _targetPercent == _dimmingPercent;
}

/**
 * Is already off?
 * @return bool true if light is off
 */
bool Light::isAlreadyOff() {
    return _targetPercent == 0;
}

/**
 * Start smooth dimming
 */
void Light::startSmoothDimming() {
    Serial.println("Light startSmoothDimming from "+String(_currentPercent)+" to "+String(_targetPercent));
    if((int)_currentPercent != _targetPercent){
        _lastUpdateTime = millis();
        float delta = _targetPercent - _currentPercent;
        _incrementPerMillisecond = delta / (_dimmingDuration * 1000);
    }
}

/**
 * Set light off
 */
void Light::setOff() {
    if(isAlreadyOff()) return;
    setPercent(0);
}

/**
 * Is light on?
 * @return bool true if light is on
 */
bool Light::isOn() {
    return !isOff();
}

/**
 * Is light off?
 * @return bool true if light is off
 */
bool Light::isOff() {
    return _targetPercent == 0;
}

/**
 * Set dimming percent
 * @param percent Int new percent value
 */
void Light::setDimmingPercent(int percent) {
    if(_dimmingPercent != percent){
        _dimmingPercent = percent;
        changePercent(percent);
    }
}

/**
 * Get dimming percent
 * @return Int current dimming percent value
 */
int Light::getDimmingPercent() {
    return _dimmingPercent;
}

/**
 * Set dimming duration
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
 */
void Light::loop()
{
    if(_currentPercent == _targetPercent) {
        return;
    }

    long loopTime = millis();
    float millisSinceLastUpdate = (loopTime - _lastUpdateTime);
    _currentPercent += _incrementPerMillisecond * millisSinceLastUpdate;
    if(_incrementPerMillisecond > 0) {
        if(_currentPercent > _targetPercent) {
            _currentPercent = _targetPercent;
        }
    } else {
        if(_currentPercent < _targetPercent) {
            _currentPercent = _targetPercent;
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
        float pwm = _currentPercent;
        pwm *= 255.0;
        pwm /= 100.0;
        Particle.publish("DEBUG", "outputPWM analog: "+String((int)pwm), 60, PRIVATE);
        analogWrite(_pin, (int) pwm);
    } else {
        bool isOn = _currentPercent > 49;
        //Particle.publish("DEBUG", "outputPWM digital isOn: "+String(isOn), 60, PRIVATE);
        bool isHigh = (isOn && !_isInverted) || (!isOn && _isInverted);
        Particle.publish("DEBUG", "outputPWM digital isHigh: "+String(isHigh), 60, PRIVATE);
        digitalWrite(_pin, isHigh ? HIGH : LOW);
    }
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