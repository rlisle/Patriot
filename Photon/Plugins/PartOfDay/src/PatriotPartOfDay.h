/**
PatriotPartOfDay plugin

Features:
- Broadcasts the current part of the day
 
 - Periods can be (in order):
    Sunrise
    Morning
    Noon
    Afternoon
    Sunset
    Dusk
    Night
    Dawn
 
 So dark out is PartOfDay > Sunset
 and light out is PartOfDay < Dusk
 
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

public:
    Period(int hour, int minute);
    
    // minute can be < 0 or > 59 and will be corrected
    void    set(int hour, int minute);
    
    bool    operator ==(const Period& hm);
    bool    operator >(const Period& hm);
    bool    operator <(const Period& hm);
};

class PartOfDay : public Device
{
private:
    long       _lastPollTime;
    long       _lastPollDay;
    Period     _periods[8];

    bool       isNextMinute();
    bool       isNextDay();
    void       calcSunriseSunset();
    int        calcPartOfDay();
    void       publish(int partOfDay);
    
public:
    int        _current;
    
    PartOfDay();
    
    void begin();
    void loop();
    
};
