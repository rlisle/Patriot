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
 
Sunrise/Sunset calculation copied from TimeLord

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

*/
#pragma once

#include "Particle.h"
#include <device.h>

class PartOfDay : public Device
{
private:
    unsigned long  _lastPollDayUTC;    // seconds since 1/1/1970 UTC
    unsigned long  _lastPollMinuteUTC; // seconds since 1/1/1970 UTC
    
    int        _sunriseMinutesAfterMidnight;
    int        _sunsetMinutesAfterMidnight;

    void       calcSunriseSunset(unsigned long currentTimeUTC);
    int        calcPartOfDay(int minutesSinceMidnight);
    void       publishPOD(int partOfDay);
    int        absolute(int n);
    bool       computeSun(int currentTimeUTC, bool forSunrise);
    void       adjust(long offset, bool forSunrise);
    char       signum(int n);

public:

    PartOfDay();
    
    void begin();
    void loop();
    
};
