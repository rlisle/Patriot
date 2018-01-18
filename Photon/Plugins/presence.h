/******************************************************************
Presence Detection

This class works in conjunction with the Proximity class to
determine and report presence.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2018-01-18: Add name and type properties
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle library
2016-11-25: Initial version
******************************************************************/
#pragma once

#include "proximity.h"

class Presence
{
private:
  bool    _isPresent;
  int     _minInches;
  int     _maxInches;
  String  _event;
  long    _interval;
  long    _lastPingTime;
  int     _shutOffDelay;
  long    _lastPresentTime;

  bool    isTimeToPing(long currentTime);
  void    publishPresenceDetected(bool present);

public:
  Presence(String name, int minInches, int maxInches, String event, long interval);
  bool loop(Proximity *proximity);

};
