/******************************************************************
 ambient light level

 Features:
 - Periodically publishes ambient light level

 http://www.github.com/rlisle/rv-arduino-libraries

 Written by Ron Lisle, ron@lisles.net

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2016-11-29: Initial version
 ******************************************************************/

#ifndef ambientlight_h
#define ambientlight_h

#include "application.h"

class IoT;

class AmbientLight
{
  friend IoT;

public:
  int       level;

  AmbientLight(int pin, int minR, int maxR);
  void setInterval(long interval);
  void setText(String eventText);

private:
  int       pin;
  int       minR;
  int       maxR;
  long      lastLoopTime;
  long      lastChangeTime;
  String    eventText;
  long      interval;

  int       reading;
  int       previousReading = 0;

  void      loop();
  int       convertReadingToLevel();
  void      publishLightLevel();

};

#endif /* ambientlight_h */
