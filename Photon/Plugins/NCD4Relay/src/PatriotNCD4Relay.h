/**
 NCD 4 Relay control
 
 Features:
 - Supports up to 4 relays on 4 R 4 GPIO board
 - On/Off control

 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2022-01-08: Initial creation based on curtian and NCD8Relay plugins
*/

#pragma once

#include "Particle.h"
#include <device.h>

class NCD4Relay : public Device
{
 private:
    int8_t        _relayNum;
    static int8_t _currentState;
    static int8_t _address;           // Addresses of board

    void   initializeBoard();

 public:
    NCD4Relay(int8_t address, int8_t relayNum, String name, String room);
    
    void    begin();

    void    setValue(int value);
    void    setOn();
    void    setOff();

    void    loop();
};
