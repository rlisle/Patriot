/******************************************************************
 Switches collection

 Features:
 - Manages a collection of Switch objects.

 http://www.github.com/rlisle/rv-arduino-libraries

 Written by Ron Lisle, ron@lisles.net

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2016-07-30: Refactor to separate switch and switches classes
 2016-06-21: Update for Photon
 2016-02-3: Initial version
 ******************************************************************/

#include "switches.h"

int Switches::addSwitch(int pinNum, String name) {
    if(_numSwitches > kMaxNumSwitches-1) {
      Serial.println("Error: addSwitch called too many times");
      Particle.publish("ERROR","addSwitch called too many times");
      return -1;
    }
    _switchArray[_numSwitches] = new Switch(pinNum, name);
    return ++_numSwitches;
}

void Switches::loop() {
  for (int i=0; i<_numSwitches; i++) {
    _switchArray[i]->loop();
  }
}
