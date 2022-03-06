/******************************************************************
 HueLight plugin

Features:
- Control Philips Hue light
 
Note: Name must match that used in the Philips Hue app.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

******************************************************************/
#pragma once

#include "Particle.h"
#include <device.h>

class HueLight : public Device
{
private:
    String    _userID;
    byte      _server[4];
    TCPClient _tcpClient;
    
    void      writeToHue();
    void      notify();
    
public:
    HueLight(String name, String room, byte server[4], String userid);

    void begin();
    void loop();
    
    void setValue(int percent);
};
