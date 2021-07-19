/**
 Patriot NCD4 Switch

 This plugin supports connection of switches to an NCD4 Relay + GPIO board.

 Features:
 - Read and debounce switch input
 
 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2021-01-18: Initial creation
*/

#pragma once

#include "Particle.h"
#include <device.h>

class NCD4Switch : public Device
{
 private:
    long    _lastPollTime;
    int8_t  _filter;

    int8_t  _boardAddress;
    int8_t  _switchBitmap;
    
    bool      isSwitchOn();
    bool      isTimeToCheckSwitch();
    bool      didSwitchChange();
    void      notify();

 public:
    NCD4Switch(int8_t boardAddress, int8_t switchIndex, String name, String room);
    
    void    begin();
    void    reset();
    void    loop();
};
