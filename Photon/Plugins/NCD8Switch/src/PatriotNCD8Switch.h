/**
 NCD GPIO Switch control

 Features:
 - Switch inputs converted to patriot MQTT messages
 - Supports multiple boards

 - Supports MCP23008
 
 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2021-06-20: Initial creation
 2022-12-10: Change MQTT message to patriot/<device>/get/position
 */

#pragma once

#include "Particle.h"
#include <device.h>

class NCD8Switch : public Device
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
    NCD8Switch(int address, int switchIndex, String name, String room);
    
    void    begin();
    int     initializeBoard();
    void    reset();
    void    loop();
    
    // Override to prevent MQTT from setting _percent.
    // Needed because of no auto behavior.
    // Our own generated MQTT message will reset _percent back to 0.
    void setPercent(int percent) { return; };
};
