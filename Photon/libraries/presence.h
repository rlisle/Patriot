/******************************************************************
  Presence Detection

 This class works in conjunction with the Proximity class to
 determine and report presence.

 http://www.github.com/rlisle/rv-arduino-libraries

 Written by Ron Lisle, ron@lisles.net

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2016-11-25: Initial version
 ******************************************************************/

#ifndef presence_h
#define presence_h

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
  Presence(int minInches, int maxInches, String event, long interval);
  bool loop(Proximity *proximity);

};

#endif
