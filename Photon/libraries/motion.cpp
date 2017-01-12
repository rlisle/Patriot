/******************************************************************
 motion sensor control

 Features:
 - Reports motion

 http://www.github.com/rlisle/rv-arduino-libraries

 Written by Ron Lisle, ron@lisles.net

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2016-07-16: Initial version
 ******************************************************************/

#include "motion.h"

Motion::Motion(int pin, const char *nm)
{
  _pin                      = pin;
  name                      = String(nm);
  pinMode(pin, INPUT);
}

void Light::loop()
{
  int current = (int)_currentLevel;
  if(current == _targetLevel) {
    return;
  }

  long loopTime = millis();
  float millisSinceLastUpdate = (loopTime - _lastUpdateTime);
  _currentLevel += _incrementPerMillisecond * millisSinceLastUpdate;
  if(_incrementPerMillisecond > 0) {
    if(_currentLevel > _targetLevel) {
      _currentLevel = _targetLevel;
    }
  } else {
    if(_currentLevel < _targetLevel) {
      _currentLevel = _targetLevel;
    }
  }
  _lastUpdateTime = loopTime;
  analogWrite(_pin,(int)_currentLevel);
};
