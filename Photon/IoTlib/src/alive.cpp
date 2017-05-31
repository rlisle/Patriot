/******************************************************************
 alive control

 Features:
 - Periodically publish alive event
 - The self registration code may use this to become aware of
   this controller.

 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle

 BSD license, check LICENSE for more information.
 All text above must be included in any redistribution.

 Changelog:
 2017-03-24: Rename Patriot
 2017-03-05: Convert to v2 particle library
 2016-06-21: Initial version
 ******************************************************************/

#include "alive.h"

Alive::Alive()
{
    this->_lastLoopTime   = 0;
}

void Alive::setPublishName(String publishName) {
    this->_publishName = publishName;
}

void Alive::setControllerName(String controllerName) {
    this->_controllerName = controllerName;
}

void Alive::loop() {
  long loopTime = millis();
  if(loopTime > _lastLoopTime + kDefaultAliveInterval) {
    _lastLoopTime = loopTime;
    Particle.publish(_publishName, (_controllerName+":alive"), 60, PRIVATE);
  }
}
