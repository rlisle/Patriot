/**
PatriotPartOfDay plugin

 Features:
 - Broadcasts the current part of the day
 
 - _value will be a value of 0 to 7 as listed below
  
  - Periods can be (in podNum order):
    1 Sunrise
    2 Morning
    3 Noon
    4 Afternoon
    5 Sunset
    6 Dusk
    7 Night
    8 Dawn

 - podNum values are defined in IoT.h
 
  Sunrise, noon, and sunset only occur for 1 minute
  
http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

*/

#include <TimeLord.h>
#include "PatriotPartOfDay.h"
#include "IoT.h"

#define MILLIS_PER_MINUTE 60000
#define MILLIS_PER_DAY 86400000

// Austin lat/long: 30.2672° N, 97.7431° W
float const LONGITUDE = -97.733330;
float const LATITUDE =  30.266666;

/**
 * Constructor
 */
PartOfDay::PartOfDay()
        : Device("PartOfDay")
{
    _value = -1;
    _type  = 'P';
}

/**
 begin is called after publishPtr is set, so we can publish her but not in constructor
 */
void PartOfDay::begin() {
    // Force next loop to perform both
    _lastPollTime = 0;
    _month = 0;
    _day = 0;
}

/**
 * loop()
 * This is called repeatedly to allow device to do its work.
 */
void PartOfDay::loop()
{
    if(isNextMinute())
    {
        if(isNextDay()) {
            calcSunriseSunset();
        }
        
        if( Time.minute() % 15 == 0 ) {
            Log("The time now is %d:%d",Time.hour(),Time.minute());
        }

        int now = calcPartOfDay();
        if (now != _value) {
            Log.info("PartOfDay changed to %d", now);
            _value = now;
            publishPOD(_value);
        }
    }
}


// Private Helper Methods
bool PartOfDay::isNextMinute()
{
    unsigned long currentTime = millis();
    if (currentTime < _lastPollTime + MILLIS_PER_MINUTE)
    {
        return false;
    }
    _lastPollTime = currentTime;
    return true;
}

bool PartOfDay::isNextDay()
{
    if( Time.day() == _day && Time.month() == _month ) return false;
    
    Log.info("DEBUG: month/day = %d/%d, Time says %d/%d",_month,_day,Time.month(),Time.day());
    
    _month = Time.month();
    _day = Time.day();

    Log.info("PartOfDay is next day %d/%d",_month,_day);
    
    return true;
}

void PartOfDay::calcSunriseSunset()
{
    // store today's date (at noon) in an array for TimeLord to use
    byte bSunrise[] = {  0, 0, 12, 27, 12, 20 };
    byte bSunset[] = { 0, 0, 12, 27, 12, 20 };

    TimeLord timeLord;
    timeLord.TimeZone(-6 * 60);
    timeLord.Position(LATITUDE, LONGITUDE);

    bSunrise[3] = Time.day();
    bSunrise[4] = Time.month();
    bSunrise[5] = Time.year();
    timeLord.SunRise(bSunrise);
    
    bSunset[3] = Time.day();
    bSunset[4] = Time.month();
    bSunset[5] = Time.year();
    timeLord.SunSet(bSunset);
    
    int sunriseHour = bSunrise[tl_hour];
    int sunriseMinute = bSunrise[tl_minute];
    int sunsetHour = bSunset[tl_hour];
    int sunsetMinute = bSunset[tl_minute];
    
    Log.info("Sunrise today %d/%d is %d:%d",Time.month(), Time.day(), sunriseHour, sunriseMinute);

    Log.info("Sunset today %d/%d is %d:%d",Time.month(), Time.day(), sunsetHour, sunsetMinute);

    _periods[SUNRISE-1].set(sunriseHour, sunriseMinute);
    _periods[MORNING-1].set(sunriseHour, sunriseMinute+1);
    _periods[NOON-1].set(12,0);
    _periods[AFTERNOON-1].set(12,1);
    _periods[SUNSET-1].set(sunsetHour, sunsetMinute);
    _periods[DUSK-1].set(sunsetHour, sunsetMinute+1);
    _periods[NIGHT-1].set(sunsetHour, sunsetMinute+30);
    _periods[DAWN-1].set(sunriseHour, sunriseMinute - 30);
}

int PartOfDay::calcPartOfDay()
{
    Period current(Time.hour(),Time.minute());
    
    if (current > _periods[NIGHT-1]) return NIGHT;
    if (current > _periods[DUSK-1]) return DUSK;
    if (current > _periods[SUNSET-1]) return SUNSET;
    if (current > _periods[AFTERNOON-1]) return AFTERNOON;
    if (current > _periods[NOON-1]) return NOON;
    if (current > _periods[MORNING-1]) return MORNING;
    if (current > _periods[SUNRISE-1]) return SUNRISE;
    if (current > _periods[DAWN-1]) return DAWN;
    return NIGHT;
}

void PartOfDay::publishPOD(int partOfDay) {
    IoT::publishValue("partofday",partOfDay);
}
