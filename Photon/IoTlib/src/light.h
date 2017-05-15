/******************************************************************
 light LED dimming control

 Features:
 - On/Off control
 - Smooth dimming

 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2017-03-24: Rename Patriot
2016-07-07: Remove dimming mode
 2016-01-17: Initial version
 ******************************************************************/

#pragma once

#include "device.h"
#include "behaviors.h"

class Light : public Device
{
private:
  Behaviors *_behaviors;
  String    _name;
  int       _pin;
  int       _dimmingPercent;
  float     _dimmingDuration;
  float     _currentPercent;
  int       _targetPercent;
  int       _commandPercent;
  float     _incrementPerMillisecond;
  long      _lastUpdateTime;

  bool      isAlreadyOn();
  bool      isAlreadyOff();
  void      changePercent(int percent);
  void      startSmoothDimming();
  void      outputPWM();
  static bool isPwmPin(int pin);

public:
  Light(int pin, String name);

  String    name();
  void      setPercent(int percent);
  int       getPercent();
  int       convertCommandToPercent(String command);
  void      setOn();
  void      setOff();
  bool      isOn();
  bool      isOff();

  void      setDimmingPercent(int percent);
  void      setDimmingDuration(float duration);
  int       getDimmingPercent();
  float     getDimmingDuration();

  void      loop();
  int       addBehavior(Behavior* behavior);
  void      performActivities(Activities* activities);

};
