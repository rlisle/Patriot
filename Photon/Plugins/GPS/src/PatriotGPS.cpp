/**
PatriotGPS plugin

 Features:
 - Broadcasts the current location
 - Interfaces to the NEO-6M GPS Module
 - Refer to https://www.electronicwings.com/sensors-modules/gps-receiver-module
     and https://www.electronicwings.com/particle/gps-module-interfacing-with-particle-photon
 
 - _value will be TBD, but probably latitude, longitude, and timezone
    
http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

*/

#include "PatriotGPS.h"
#include "IoT.h"

#define MILLIS_PER_MINUTE 60000
#define MILLIS_PER_DAY 86400000

// Spanish Fort, AL: 30.6685° N, 87.9109° W
// Bonifay, FL: 30.7919° N, 85.6797° W
// White Springs, FL: 30.3297° N, 82.7590° W
// Tampa, FL: 27.9506° N, 82.4572° W
// Austin lat/long: 30.2672° N, 97.7431° W (30.266666, -97.733330)
//                  30.28267 N, 97.63624 W via iPhone maps in office.
float const LATITUDE =  30.28267;
float const LONGITUDE = -97.63624;

/**
 * Constructor
 */
GPS::GPS()
        : Device("GPS", "All")
{
    _value = -1;
    _type  = 'G';
}

/**
 begin is called after publishPtr is set, so we can publish her but not in constructor
 */
void GPS::begin() {
    // Force next loop to perform both
    _lastPollTime = 0;
}

/**
 * loop()
 * This is called repeatedly to allow device to do its work.
 */
void GPS::loop()
{
    if(isNextMinute())
    {
        //TODO:
    }
}


// Private Helper Methods
bool GPS::isNextMinute()
{
    unsigned long currentTime = millis();
    if (currentTime < _lastPollTime + MILLIS_PER_MINUTE)
    {
        return false;
    }
    _lastPollTime = currentTime;
    return true;
}

void PartOfDay::publishGPS() {
    IoT::publishValue("latitude","todo");
    IoT::publishValue("longitude","todo");
    IoT::publishValue("timezone","todo");
}
