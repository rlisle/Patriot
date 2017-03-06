/******************************************************************
Presence Detector

TODO: convert from on/off (0,100) to 0 or filtered inches

http://www.github.com/rlisle/ParticleIoT

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-05: Convert to v2 particle library
2017-02-04: Convert from on/off to percent
2016-11-25: Initial version
******************************************************************/
#include "application.h"
#include "presence.h"
#include "proximity.h"

#define kDefaultShutOffDelay 5000    // 5 seconds

Presence::Presence(int min, int max, String event, long interval)
{
  _isPresent        = false;
  _lastPingTime     = 0;
  _minInches        = min;
  _maxInches        = max;
  _event            = event;
  _interval         = interval;
  _shutOffDelay     = kDefaultShutOffDelay;
  _lastPresentTime  = 0;
}

bool Presence::loop(Proximity *proximity)
{
  long now = millis();
  if(isTimeToPing(now)) {
    int inches = proximity->ping();
    if(inches < _maxInches && inches > _minInches) {
      _lastPresentTime = now;
      if(!_isPresent) {
        publishPresenceDetected(true);
      }
    } else {
      if(_isPresent &&
        (now > _lastPresentTime + _shutOffDelay)) {
        publishPresenceDetected(false);
      }
    }
    _lastPingTime = now;
  }
  return _isPresent;
}

bool Presence::isTimeToPing(long currentTime)
{
  return (currentTime >= _lastPingTime + _interval);
}

void Presence::publishPresenceDetected(bool present) {
  _isPresent = present;
  String data = present ? _event + ":100" : _event + ":0";
  Serial.println("Presence change: "+data);
  Particle.publish("lislerv", data);
}
