/**
PatriotDateTime plugin

Features:
- Broadcasts date and time messages

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

*/
#pragma once

#include "Particle.h"
#include "device.h"

class DateTime : public Device
{
private:
    long       _lastPollTime;

    bool      isTimeToUpdate();
    void      notify();
    
public:
    DateTime();

    void loop();
    
};
