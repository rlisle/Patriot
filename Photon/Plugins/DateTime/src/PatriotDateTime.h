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

class HourMinute {
    int     _hour;
    int     _minute;
    
    HourMinute(int hour, int minute);
    
    bool    operator ==(const HourMinute& hm);
    bool    operator >(const HourMinute& hm);
    bool    operator <(const HourMinute& hm);
};

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
