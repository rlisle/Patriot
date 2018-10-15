/******************************************************************
 Smartthings light LED dimming control

 Features:
 - On/Off control
 - Smooth dimming with duration

 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2018-10-15: Initial version
 ******************************************************************/

#include "PatriotSTLight.h"

/**
 * Constructor
 * @param stname String name used defined by Smartthings.
 * @param name String name used to address the light by Alexa & Patriot.
 * @param forceDigital True if output On/Off only
 */
STLight::STLight(String stname, String name, bool forceDigital)
        : _stname(stname),
          Device(name, DeviceType::Light),
          _forceDigital(forceDigital)
{
    _dimmingPercent           = 100;                                // On full
    _dimmingDuration          = 2.0;
    _currentPercent           = 0.0;
    _targetPercent            = 0;
    _incrementPerMillisecond  = 0.0;
    _lastUpdateTime           = 0;
    _commandPercent            = 0;
}

/**
 * Set percent
 * @param percent Int 0 to 100
 */
void Light::setPercent(int percent) {
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
    if(_targetPercent == percent) return;

    _targetPercent = percent;
    if(_dimmingDuration == 0.0 || isDimmingSupported() == false) {
        _currentPercent = percent;
        outputPWM();

    } else {
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
    outputPercent();
};

/**
 * Set the output percent value (0-100)
 */
void Light::outputPercent() {
    if(isDimmingSupported()) {
        //TODO: Write _currentPercent to MQTT
    } else {
        bool isOn = _currentPercent > 49;
        //TODO: Write isOn to MQTT
    }
}

/**
 * Is Dimming supported?
 * @return bool device supports dimming
 */
bool Light::isDimmingSupported()
{
    //TODO: query device
    return true;
}