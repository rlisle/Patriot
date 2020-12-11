/**
 NCD GPIO Switch control

 Features:
 - Switch inputs converted to patriot MQTT messages
 - Supports multiple boards

 - Supports MCP23017
 
 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

*/

#pragma once

#include "Particle.h"
#include "device.h"

class NCD16Switch : public Device
{
private:
    int     _address;
    int     _switchBitmap;
    int     _lastState;     // Bitmap of 16 switches
    long    _lastPollTime;
    bool    _isOn;          // current/last switch state
    
    int    initializeBoard();
    bool   isOn();          // Read the switch state

public:
    NCD16Switch(int address, int switchNum, String name);
    
    void    loop();
    
    bool shouldAutoCreateBehavior() { return false; };
    
    // Override to prevent MQTT from setting _percent.
    // Needed because of no auto behavior.
    // Our own generated MQTT message will reset _percent back to 0.
    void setPercent(int percent) { return; };
};
