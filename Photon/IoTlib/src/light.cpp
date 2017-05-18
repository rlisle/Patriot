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
 2017-05-15: Make devices generic
 2017-03-24: Rename Patriot
 2016-07-07: Remove dimming mode
 2016-01-17: Initial version
 ******************************************************************/

#include "light.h"

Light::Light(int pin, String name)
{
  Serial.println("Create light "+name+" on pin "+String(pin));

//  _behaviors                = new Behaviors();
  _pin                      = pin;
  _name                     = name;
  _dimmingPercent           = 100;  // On full
  _dimmingDuration          = Light::isPwmPin(pin) ? 2.0 : 0;
  _currentPercent           = 0.0;
  _targetPercent            = 0;
  _incrementPerMillisecond  = 0.0;
  _lastUpdateTime           = 0;
  _commandPercent            = 0;
  pinMode(pin, OUTPUT);
}

String Light::name() {
  return _name;
}

// Handle 'On', 'Off', 'Toggle', '+', '-', or 0-100
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

void Light::setPercent(int percent) {
  _commandPercent = percent;
  changePercent(percent);
}

int Light::getPercent() {
  return _currentPercent;
}

void Light::setOn() {
  if(isAlreadyOn()) return;
  changePercent(_dimmingPercent);
}

void Light::changePercent(int percent) {
  if(_targetPercent == percent) return;

  _targetPercent = percent;
  if(_dimmingDuration == 0.0 || !Light::isPwmPin(_pin)) {
    Serial.println("Light setting percent to "+String(percent));
    _currentPercent = percent;
      if(Light::isPwmPin(_pin)){
          analogWrite(_pin,percent);
      } else {
          digitalWrite(_pin, (percent > 49) ? HIGH : LOW);
      }

  } else {
    startSmoothDimming();
  }
}

bool Light::isAlreadyOn() {
  return _targetPercent == _dimmingPercent;
}

bool Light::isAlreadyOff() {
  return _targetPercent == 0;
}

void Light::startSmoothDimming() {
Serial.println("Light startSmoothDimming from "+String(_currentPercent)+" to "+String(_targetPercent));
  if((int)_currentPercent != _targetPercent){
    _lastUpdateTime = millis();
    float delta = _targetPercent - _currentPercent;
    _incrementPerMillisecond = delta / (_dimmingDuration * 1000);
  }
}

void Light::setOff() {
  if(isAlreadyOff()) return;
  setPercent(0);
}

bool Light::isOn() {
  return !isOff();
}

bool Light::isOff() {
  return _targetPercent == 0;
}

void Light::setDimmingPercent(int percent) {
  if(_dimmingPercent != percent){
    _dimmingPercent = percent;
    changePercent(percent);
  }
}

int Light::getDimmingPercent() {
  return _dimmingPercent;
}

void Light::setDimmingDuration(float duration) {
  _dimmingDuration = duration;
}

float Light::getDimmingDuration() {
  return _dimmingDuration;
}

// private methods
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

void Light::outputPWM() {
    if(Light::isPwmPin(_pin)) {
        float pwm = _currentPercent;
        pwm *= 255.0;
        pwm /= 100.0;
//        Serial.println("outputPWM: " + String(pwm) + " on pin " + String(_pin));
        analogWrite(_pin, (int) pwm);
    } else {
        digitalWrite(_pin, (_currentPercent > 49) ? HIGH : LOW);
    }
}

//int Light::addBehavior(Behavior *behavior)
//{
//  return _behaviors->addBehavior(behavior);
//}

//void Light::performActivities(Activities* activities)
//{
//    int percent = _behaviors->determineLevelForActivities(_commandPercent, activities);
//    Serial.println("Light "+_name+" performActivities: setting "+String(percent));
//    changePercent(percent);
//}

//TODO: Move this up to parent class Device
bool Light::isPwmPin(int pin)
{
    switch(pin) {
        case D0:
        case D1:
        case D2:
        case D3:
        case A4:
        case A5:
        case A7:    // aka WKP
        case RX:
        case TX:
            return TRUE;
        default:
            break;
    };
    return FALSE;
}