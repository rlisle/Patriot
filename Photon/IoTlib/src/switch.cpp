/******************************************************************
Switch control

This class represents a single switch.
Switches can optionally be mapped to activities.

Features:
- Read and debounce switch inputs
- Map to activity
- Publish changes

Assumptions:
- loop() is called every 50 msecs by Switches, eliminating need
 for debouncing.
TODO: Move debouncing to individual switch instead of switches.

http://www.github.com/rlisle/ParticleIoT

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-05: Convert to v2 particle library
2016-07-29: Refactor to separate switch and switches classes
           Map switch events to activities
2016-06-21: Update for Photon
2016-02-3: Initial version
******************************************************************/
#include "switch.h"

#define kDebounceDelay  500    // 50 msec delay for switch to settle down

Switch::Switch(int pinNum, String name)
    : _pin(pinNum), _name(name)
{
    pinMode(pinNum, INPUT_PULLUP);
    _lastReadTime = millis();
}

bool Switch::isOn() {
  return _isOn;
}

void Switch::loop() {
  if(isTimeToCheckSwitch()) {
    if(didSwitchChange()) {
      notify();
    }
  }
}

// Private Helper Methods
bool Switch::isTimeToCheckSwitch() {
  long currentTime = millis();
  if(currentTime < _lastReadTime + kDebounceDelay) {
    return false;
  }
  _lastReadTime = currentTime;
  return true;
}

bool Switch::didSwitchChange() {
  bool newState = digitalRead(_pin) == 0;
  if(newState == _isOn) {
    return false;
  }
  _isOn = newState;
  return true;
}

void Switch::notify() {
  String pubString = _name + ":" + (_isOn ? "100" : "0");
  Serial.println(pubString);
  Particle.publish("lislerv",pubString);
}
