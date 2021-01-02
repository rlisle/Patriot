/**
PatriotPartOfDay plugin

 Features:
 - Broadcasts the current part of the day
 
 - _value will be a value of 0 to 7 as listed below
  
  - Periods can be (in podNum order):
    0 Sunrise
    1 Morning
    2 Noon
    3 Afternoon
    4 Sunset
    5 Dusk
    6 Night
    7 Dawn

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

Period::Period(int hour, int minute) {
    _hour = hour;
    _minute = minute;
}

// hour and minute can be < 0 or too big and will be corrected
void Period::set(int hour, int minute) {
    _hour = hour;
    _minute = minute;
    if(_minute < 0) {
        _hour--;
        _minute += 60;
    }
    if(_minute >= 60) {
        _hour++;
        _minute -= 60;
    }
    if(_hour >= 24) {
        _hour -= 24;
    }
    if(_hour < 0) {
        hour += 24;
    }
}

bool Period::operator ==(const Period& period) {
    return period._hour == _hour && period._minute == _minute;
}

bool Period::operator >(const Period& period) {
    if(_hour > period._hour) return true;
    if(_hour < period._hour) return false;
    return _minute > period._minute;
}

bool Period::operator <(const Period& period) {
    if(_hour < period._hour) return true;
    if(_hour > period._hour) return false;
    return _minute < period._minute;
}

String Period::info() {
    return String(_hour) + ":" + String(_minute);
}

/**
 * Constructor
 */
PartOfDay::PartOfDay()
        : Device("PartOfDay")
{
    _value = -1;
}

/**
 begin is called after publishPtr is set, so we can publish her but not in constructor
 */
void PartOfDay::begin() {
    // Force next loop to perform both
    _lastPollTime = 0;
    _lastPollDay = 0;
    calcSunriseSunset();
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
            Log.info("PartOfDay next day");
            calcSunriseSunset();
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
    unsigned long currentTime = millis();
    if (currentTime < _lastPollDay + MILLIS_PER_DAY)
    {
        return false;
    }
    _lastPollDay = currentTime;
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

    _periods[SUNRISE].set(sunriseHour, sunriseMinute);
    _periods[MORNING].set(sunriseHour, sunriseMinute+1);
    _periods[NOON].set(12,0);
    _periods[AFTERNOON].set(12,1);
    _periods[SUNSET].set(sunsetHour, sunsetMinute);
    _periods[DUSK].set(sunsetHour, sunsetMinute+1);
    _periods[NIGHT].set(sunsetHour, sunsetMinute+30);
    _periods[DAWN].set(sunriseHour, sunriseMinute - 30);
}

int PartOfDay::calcPartOfDay()
{
    Period current(Time.hour(),Time.minute());
    Log("The time now is " + String(current.info()));
    
    if (current > _periods[NIGHT]) return NIGHT;
    if (current > _periods[DUSK]) return DUSK;
    if (current > _periods[SUNSET]) return SUNSET;
    if (current > _periods[AFTERNOON]) return AFTERNOON;
    if (current > _periods[NOON]) return NOON;
    if (current > _periods[MORNING]) return MORNING;
    if (current > _periods[SUNRISE]) return SUNRISE;
    if (current > _periods[DAWN]) return DAWN;
    return NIGHT;
}

void PartOfDay::publishPOD(int partOfDay) {
    IoT::publishValue("partofday",partOfDay);
}
