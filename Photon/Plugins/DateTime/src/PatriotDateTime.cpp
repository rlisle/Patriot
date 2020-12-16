/**
PatriotDateTime plugin

Features:
- Broadcasts date and time messages

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

*/

#include "PatriotDateTime.h"

#define MILLIS_PER_SECOND 1000
// Update each minute
#define POLL_INTERVAL_MILLIS 60000

/**
 * Constructor
 */
DateTime::DateTime()
        : Device("DateTime", DeviceType::DateTime)
{
    _percent = 0;
    _lastPollTime = millis();
}


/**
 * loop()
 * This is called repeatedly to allow device to do its work.
 */
void DateTime::loop()
{
    if (isTimeToUpdate())
    {
        publishMinutes();
    }
}


// Private Helper Methods
/**
 * isTimeToUpdate()
 * @return bool if enough time has elapsed to sample switch again
 */
bool DateTime::isTimeUpdate()
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
 * publishMinutes()
 * Publish current minutes
 */
void DateTime::publishMinutes()
{
    String minutes = Time.format(Time.now(), "%M");
    String topic = "patriot/" + _name;
//    String message = String(_percent);
//    publish(topic,message);
}
