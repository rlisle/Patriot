/******************************************************************
 NCD Relay control

 Features:
 - On/Off control
 - Supports multiple boards

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
    int8_t  _relayNum;
    int8_t  _boardIndex;            // Index into static arrays
    int8_t  _registerAddress;       // Is this different for different boards?
    int8_t  _duration;              // # seconds to keep on. 0 = forever
    long    _stopMillis;            // time in millis to auto-stop

    static int8_t _numControllers;    // Count of relay boards on I2C bus
    static int8_t _currentStates[];   // up to 8 relays currently supported
    static int8_t _addresses[];       // Addresses of up to 8 boards

    int8_t  initialize8RelayBoard(int8_t address);
    int8_t  initializeBoard(int8_t address);
    int8_t  boardIndex(int8_t address);
    int8_t  addAddressToArray(int8_t address);

 public:
    Relay(int8_t address, int8_t numRelays, int8_t relayNum, String name, int8_t duration = 0);

    String  name();
    void    setPercent(int percent);
    void    setOn();
    void    setOff();
    bool    isOn();
    bool    isOff();

    void    loop();
};
