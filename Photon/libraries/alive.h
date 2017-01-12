/******************************************************************
 alive control

 Features:
 - Periodically publish alive event
 - Very small code

 http://www.github.com/rlisle/rv-arduino-libraries

 Written by Ron Lisle, ron@lisles.net

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2016-06-21: Initial version
 ******************************************************************/

#ifndef alive_h
#define alive_h

#include "application.h"
#include "LisleRV.h"

class IoT;

class Alive
{
  friend IoT;

public:
  Alive(String name);

private:
  long      _lastLoopTime;
  String    _name;

  void      loop();
};

#endif /* alive_h */
