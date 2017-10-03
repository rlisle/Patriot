/******************************************************************
 Relay control

 Features:
 - On/Off control

 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2017-10-03: Initial creation
 ******************************************************************/

#pragma once

#include "Particle.h"
#include "device.h"

class Relay : public Device
{
 private:
    String    _name;
    int       _relayNum;
    int       _address;

 public:
    Relay(int address, int relayNum, String name);

    String    name();
    void      setOn();
    void      setOff();
    bool      isOn();
    bool      isOff();

    void      loop();
};
