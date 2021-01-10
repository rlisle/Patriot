/**
PatriotPartOfDay plugin

Features:
- Broadcasts the current part of the day
 
 - Periods can be (in order):
    1 Sunrise
    2 Morning
    3 Noon
    4 Afternoon
    5 Sunset
    6 Dusk
    7 Night
    8 Dawn
 
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
#include <device.h>
#include "Period.h"

class PartOfDay : public Device
{
private:
    unsigned long  _lastPollTime;
    Period     _periods[8];
    int        _month;
    int        _day;

    bool       isNextMinute();
    bool       isNextDay();
    void       calcSunriseSunset();
    int        calcPartOfDay();
    void       publishPOD(int partOfDay);
    
public:

    PartOfDay();
    
    void begin();
    void loop();
    
};
