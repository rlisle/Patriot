/**
PatriotGPS plugin

Features:
- Broadcasts the current location
  
http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

*/
#pragma once

#include "Particle.h"
#include <device.h>

class GPS : public Device
{
private:
    unsigned long  _lastPollTime;

    bool       isNextMinute();
    void       publishGPS();
    
public:

    GPS();
    
    void begin();
    void loop();
    
};
