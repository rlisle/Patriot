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

#include "PatriotPartOfDay.h"

#define MILLIS_PER_SECOND 1000
// Update each minute
#define POLL_INTERVAL_MILLIS 60000


Period::Period(int hour, int minute, int podNum) {
    _hour = hour;
    _minute = minute;
    _podNum = podNum;
    
}

bool Period::operator ==(const Period& period) {
    return period._hour == _hour && period._minute == _minute;
}

bool Period::operator >(const Period& period) {
    //Log.info("Comparing period " + String(period._hour) + ":" + String(period._minute));
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
        : Device("PartOfDay", DeviceType::PartOfDay)
{
    _lastPollTime = millis();
    _current = 0;
    //_current = determine();
    //publishCurrent();     // Causing loop?
}


/**
 * loop()
 * This is called repeatedly to allow device to do its work.
 */
void PartOfDay::loop()
{
    if (isTimeToUpdate())
    {
        int now = determine();
        //Log.info("PartOfDay: time to update = " + String(now));
        if (now != _current) {
            //Log.info("PartOfDay now: " + now);
            _current = now;
            publishCurrent();
        }
    }
}


// Private Helper Methods
/**
 * isTimeToUpdate()
 * @return bool if enough time has elapsed to sample switch again
 */
bool PartOfDay::isTimeToUpdate()
{
    long currentTime = millis();
    if (currentTime < _lastPollTime + POLL_INTERVAL_MILLIS)
    {
        return false;
    }
    _lastPollTime = currentTime;
    return true;
}

/**
 * publishinutes()
 * Publish current minutes
 */
int PartOfDay::determine()
{
    Period dawn = Period(6,52,1);               // Dawn
    Period sunrise = Period(7,22,2);            // Sunrise
    Period morning = Period(7,23,3);            // Morning
    Period noon = Period(12,0,4);               // Noon
    Period afternoon = Period(12,1,5);          // Afternoon
    Period sunset = Period(17,33,6);            // Sunset
    Period dusk = Period(17,34,7);              // Dusk
    Period night = Period(18,3,0);              // Night

    Period current = Period(Time.hour(),Time.minute(), 0);
    //Log.info("PartOfDay determine: time now = " + String(Time.hour()) + ":" + String(Time.minute()));
    
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

void PartOfDay::publishCurrent() {
    String topic = "patriot/partofday";
    String message = String(_current);
    publish(topic,message);
}