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

#include "PatriotPartOfDay.h"

#define MILLIS_PER_SECOND 1000
// Update each minute
#define POLL_INTERVAL_MILLIS 60000

Period::Period(int hour, int minute, String name) {
    _hour = hour;
    _minute = minute;
    _name = name;
    
}

bool Period::operator ==(const Period& period) {
    return period._hour == _hour && period._minute == _minute;
}

bool Period::operator >(const Period& period) {
    if(period._hour > _hour) return true;
    if(period._hour < _hour) return false;
    return period._minute > _minute;
}

bool Period::operator <(const Period& period) {
    if(period._hour < _hour) return true;
    if(period._hour > _hour) return false;
    return period._minute < _minute;
}

/**
 * Constructor
 */
PartOfDay::PartOfDay()
        : Device("PartOfDay", DeviceType::PartOfDay)
{
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
    if (isTimeToUpdate())
    {
        String now = determine();
        if (now != _current) {
            Log.trace("PartOfDay now: " + now);
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
String PartOfDay::determine()
{
    Period dawn = Period(6,52,"dawn");
    Period sunrise = Period(7,22,"sunrise");
    Period morning = Period(7,23,"morning");
    Period noon = Period(12,0,"noon");
    Period afternoon = Period(12,1,"afternoon");
    Period sunset = Period(17,33,"sunset");
    Period dusk = Period(17,34,"dusk");
    Period night = Period(18,3,"night");

    Period current = Period(Time.hour(),Time.minute(), "now");
    
    if (current > night) return "night";
    if (current > dusk) return "dusk";
    if (current > night) return "sunset";
    if (current > night) return "afternoon";
    if (current > night) return "noon";
    if (current > night) return "morning";
    if (current > night) return "sunrise";
    if (current > night) return "dawn";
    if (current > night) return "Night";
    return "night";
}

void PartOfDay::publishCurrent() {
    String topic = "patriot/partofday";
    String message = String(_current);
    publish(topic,message);
}
