/******************************************************************
 Alive control

 Features:
 - Publish periodic alive events
 - Advertise controller name

 http://www.github.com/rlisle/rv-arduino-libraries

 Written by Ron Lisle, ron@lisles.net

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2017-01-05: Publish controller name
 2016-06-21: Initial version
 ******************************************************************/

#include "alive.h"

Alive::Alive(String name)
{
    this->_lastLoopTime  = 0;
    this->_name          = name;
}

void Alive::loop() {
  long loopTime = millis();
  if(loopTime > _lastLoopTime + kDefaultAliveInterval) {
    _lastLoopTime = loopTime;
    Particle.publish(kPublishName, _name+":alive");
  }
}
