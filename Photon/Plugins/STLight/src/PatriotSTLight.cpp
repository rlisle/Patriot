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

#include <IoT.h>
#include "PatriotSTLight.h"

/**
 * Constructor
 * @param stname String name used defined by Smartthings.
 * @param name String name used to address the light by Alexa & Patriot.
 * @param forceDigital True if output On/Off only
 */
STLight::STLight(String stname, String name, bool forceDigital)
        : Device(name, DeviceType::Light),
          _stname(stname),
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
 * Is Dimming supported?
 * @return bool device supports dimming
 */
bool STLight::isDimmingSupported()
{
    //TODO: query device
    return false;
}

/**
 * Set percent
 * @param percent Int 0 to 100
 */
void STLight::setPercent(int percent) {
    _commandPercent = percent;
    changePercent(percent);
}

/**
 * Get percent
 * @return Int current 0-100 percent value
 */
int STLight::getPercent() {
    return _currentPercent;
}

/**
 * Set On
 */
void STLight::setOn() {
    if(isAlreadyOn()) return;
    changePercent(_dimmingPercent);
}

/**
 * Change percent
 * @param percent Int new percent value
 */
void STLight::changePercent(int percent) {
    if(_targetPercent == percent) return;

    _targetPercent = percent;
    if(_dimmingDuration == 0.0 || isDimmingSupported() == false) {
        _currentPercent = percent;
        outputPercent();

    } else {
        startSmoothDimming();
    }
}

/**
 * Is already on?
 * @return bool true if light is on
 */
bool STLight::isAlreadyOn() {
    return _targetPercent == _dimmingPercent;
}

/**
 * Is already off?
 * @return bool true if light is off
 */
bool STLight::isAlreadyOff() {
    return _targetPercent == 0;
}

/**
 * Start smooth dimming
 */
void STLight::startSmoothDimming() {
    if((int)_currentPercent != _targetPercent){
        _lastUpdateTime = millis();
        float delta = _targetPercent - _currentPercent;
        _incrementPerMillisecond = delta / (_dimmingDuration * 1000);
    }
}

/**
 * Set light off
 */
void STLight::setOff() {
    if(isAlreadyOff()) return;
    setPercent(0);
}

/**
 * Is light on?
 * @return bool true if light is on
 */
bool STLight::isOn() {
    return !isOff();
}

/**
 * Is light off?
 * @return bool true if light is off
 */
bool STLight::isOff() {
    return _targetPercent == 0;
}

/**
 * Set dimming percent
 * @param percent Int new percent value
 */
void STLight::setDimmingPercent(int percent) {
    if(_dimmingPercent != percent){
        _dimmingPercent = percent;
        changePercent(percent);
    }
}

/**
 * Get dimming percent
 * @return Int current dimming percent value
 */
int STLight::getDimmingPercent() {
    return _dimmingPercent;
}

/**
 * Set dimming duration
 * @param duration float number of seconds
 */
void STLight::setDimmingDuration(float duration) {
    _dimmingDuration = duration;
}

/**
 * Get dimming duration
 * @return float number of dimming duration seconds
 */
float STLight::getDimmingDuration() {
    return _dimmingDuration;
}

/**
 * Private Methods
 */

/**
 * loop()
 */
void STLight::loop()
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
void STLight::outputPercent() {
    IoT *iot = IoT::getInstance();
    String topic = _stname;
    String message;
    if(isDimmingSupported()) {
        //TODO: Write switch on/off first time only

        message = String::format("%d",_currentPercent);
        topic += "/level";
        iot->mqttPublish(topic, message);
    } else {
        bool isOn = _currentPercent > 49;
        topic += "/switch";
        message = isOn ? "on" : "off";
        iot->mqttPublish(topic, message);
    }
}
