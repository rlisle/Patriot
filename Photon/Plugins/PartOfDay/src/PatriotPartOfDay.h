/**
PatriotPartOfDay plugin

Features:
- Broadcasts the current part of the day
 
 - Periods can be (in order):
    Night
    Dawn
    Sunrise
    Morning
    Noon
    Afternoon
    Sunset
    Dusk

 Sunrise, noon, and sunset only occur for 1 minute
 
http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

*/
#pragma once

#include "Particle.h"
#include "device.h"

class Period {
private:
    int     _hour;
    int     _minute;
    int     _podNum;        //TODO: change to enum

public:
    Period(int hour, int minute, int podNum);
    
    bool    operator ==(const Period& hm);
    bool    operator >(const Period& hm);
    bool    operator <(const Period& hm);
};

class PartOfDay : public Device
{
private:
    long       _lastPollTime;
//    Period     _periods[8];

    bool       isTimeToUpdate();
    int        determine();
    void       publishCurrent();
    
public:
    int        _current;
    
    PartOfDay();

    void loop();
    
};
