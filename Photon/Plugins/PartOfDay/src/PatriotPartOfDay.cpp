/**
PatriotPartOfDay plugin

 Features:
 - Broadcasts the current part of the day
  
  - Periods can be (in podNum order):
     0 Night
     1 Dawn
     2 Sunrise
     3 Morning
     4 Noon
     5 Afternoon
     6 Sunset
     7 Dusk

  Sunrise, noon, and sunset only occur for 1 minute
  
http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

*/

#include <TimeLord.h>
#include "PatriotPartOfDay.h"

#define MILLIS_PER_MINUTE 60000
#define MILLIS_PER_DAY 86400000

// Austin lat/long: 30.2672° N, 97.7431° W
float const LONGITUDE = -97.733330;
float const LATITUDE =  30.266666;

Period::Period(int hour, int minute, int podNum) {
    _hour = hour;
    _minute = minute;
    _podNum = podNum;
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

/**
 * Constructor
 */
PartOfDay::PartOfDay()
        : Device("PartOfDaySource", DeviceType::PartOfDay)
{
    _current = -1;
}

/**
 begin is called after publishPtr is set, so we can publish her but not in constructor
 */
void PartOfDay::begin() {
    _lastPollTime = millis();
    _current = determine();
    publishCurrent();
}

/**
 * loop()
 * This is called repeatedly to allow device to do its work.
 */
void PartOfDay::loop()
{
    if (_current == -1 || isNextMinute())
    {
        if( _current == -1 || isNextDay()) {
            calcSunriseSunset();
        }
        
        int now = determine();
        if (now != _current) {
            Log.info("PartOfDay changed to %d", now);
            _current = now;
            publishCurrent();
        }
    }
}


// Private Helper Methods
bool PartOfDay::isNextMinute()
{
    long currentTime = millis();
    if (currentTime < _lastPollTime + POLL_INTERVAL_MILLIS)
    {
        return false;
    }
    _lastPollTime = currentTime;
    return true;
}

bool PartOfDay::isNextDay()
{
//    long currentTime = millis();
//    if (currentTime < _lastPollTime + POLL_INTERVAL_MILLIS)
//    {
//        return false;
//    }
//    _lastPollTime = currentTime;
//    return true;
}

int PartOfDay::calcSunriseSunset()
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
    Period *sunRise = new Period(bSunrise[tl_hour], bSunrise[tl_minute], 2);

    Log.info("Sunrise today %d/%d is %d:%d",Time.month(), Time.day(), int(bSunrise[tl_hour]), int(bSunrise[tl_minute]));
    
    bSunset[3] = Time.day();
    bSunset[4] = Time.month();
    bSunset[5] = Time.year();
    timeLord.SunSet(bSunset);
    Period *sunSet = new Period(bSunset[tl_hour], bSunset[tl_minute], 2);
    
    Log.info("Sunset today %d/%d is %d:%d",Time.month(), Time.day(), int(bSunset[tl_hour]), int(bSunset[tl_minute]));
    
    //TODO: set once each day using _periods array
    Period dawn = Period(6,52,1);               // Dawn
    Period sunrise = Period(7,22,2);            // Sunrise
    Period morning = Period(7,23,3);            // Morning
    Period noon = Period(12,0,4);               // Noon
    Period afternoon = Period(12,1,5);          // Afternoon
    Period sunset = Period(17,33,6);            // Sunset
    Period dusk = Period(17,34,7);              // Dusk
    Period night = Period(18,3,0);              // Night

    Period current = Period(Time.hour(),Time.minute(), 0);
    Log.info("PartOfDay determine: time now = " + String(Time.hour()) + ":" + String(Time.minute()));
    
    if (current > night) return 0;
    if (current > dusk) return 7;
    if (current > sunset) return 6;
    if (current > afternoon) return 5;
    if (current > noon) return 4;
    if (current > morning) return 3;
    if (current > sunrise) return 2;
    if (current > dawn) return 1;
    return 0;
}

int PartOfDay::calcPartOfDay()
{
    byte bSunrise[] = {  0, 0, 12, 27, 12, 20 }; // store today's date (at noon) in an array for TimeLord to use
    byte bSunset[] = { 0, 0, 12, 27, 12, 20 };

    TimeLord tardis;
    
    tardis.TimeZone(-6 * 60); // tell TimeLord what timezone your RTC is synchronized to. You can ignore DST
                              // as long as the RTC never changes back and forth between DST and non-DST
    tardis.Position(LATITUDE, LONGITUDE); // tell TimeLord where in the world we are

    bSunrise[3] = Time.day();
    bSunrise[4] = Time.month();
    bSunrise[5] = Time.year();
    tardis.SunRise(bSunrise);
    Period *sunRise = new Period(bSunrise[tl_hour], bSunrise[tl_minute], 2);

    Log.info("Sunrise today %d/%d is %d:%d",Time.month(), Time.day(), int(bSunrise[tl_hour]), int(bSunrise[tl_minute]));
    
    bSunset[3] = Time.day();
    bSunset[4] = Time.month();
    bSunset[5] = Time.year();
    tardis.SunSet(bSunset);
    Period *sunSet = new Period(bSunset[tl_hour], bSunset[tl_minute], 2);
    
    Log.info("Sunset today %d/%d is %d:%d",Time.month(), Time.day(), int(bSunset[tl_hour]), int(bSunset[tl_minute]));
    
    //TODO: set once each day using _periods array
    Period dawn = Period(6,52,1);               // Dawn
    Period sunrise = Period(7,22,2);            // Sunrise
    Period morning = Period(7,23,3);            // Morning
    Period noon = Period(12,0,4);               // Noon
    Period afternoon = Period(12,1,5);          // Afternoon
    Period sunset = Period(17,33,6);            // Sunset
    Period dusk = Period(17,34,7);              // Dusk
    Period night = Period(18,3,0);              // Night

    Period current = Period(Time.hour(),Time.minute(), 0);
    Log.info("PartOfDay determine: time now = " + String(Time.hour()) + ":" + String(Time.minute()));
    
    if (current > night) return 0;
    if (current > dusk) return 7;
    if (current > sunset) return 6;
    if (current > afternoon) return 5;
    if (current > noon) return 4;
    if (current > morning) return 3;
    if (current > sunrise) return 2;
    if (current > dawn) return 1;
    return 0;
}

void PartOfDay::publish(int partOfDay) {
    String topic = "patriot/partofday";
    String message = String(partOfDay);
    publish(topic,message);
}
