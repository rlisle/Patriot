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
 
Uses code from TimeLord github.com/probonopd/TimeLord.git

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

*/

#include "PatriotPartOfDay.h"
#include "IoT.h"
#include "math.h"

#define MILLIS_PER_MINUTE 60000
#define MILLIS_PER_DAY 86400000

// Spanish Fort, AL: 30.6685° N, 87.9109° W
// Bonifay, FL: 30.7919° N, 85.6797° W
// White Springs, FL: 30.3297° N, 82.7590° W
// Tampa, FL: 27.9506° N, 82.4572° W
// Austin lat/long: 30.2672° N, 97.7431° W (30.266666, -97.733330)
//                  30.28267 N, 97.63624 W via iPhone maps in office.
//TODO: calculate this from GPS or app or API or ?
float const LATITUDE  =  30.28267;
float const LONGITUDE = -97.63624;
int   const TIMEZONE  = -6 * 60;

/**
 * Constructor
 */
PartOfDay::PartOfDay()
        : Device("PartOfDay", "All")
{
    _value = -1;
    _type  = 'P';
    _sunriseMinutesAfterMidnight = 400; // 6:40 am Placeholder
    _sunsetMinutesAfterMidnight = 1230; // 8:30 pm placeholder
    _latitude = LATITUDE;               // Default (Austin)
    _longitude = LONGITUDE;             // "
}

/**
 begin is called after publishPtr is set, so we can publish here but not in constructor
 */
void PartOfDay::begin() {
    // Force next loop to perform both
    _lastPollDayUTC = 0;
    _lastPollMinuteUTC = 0;
}

void PartOfDay::setLatLong(float latitude, float longitude) {
    _latitude = latitude;
    _longitude = longitude;
    _lastPollMinuteUTC = 0;     // Force recalculation
    _lastPollDayUTC = 0;
}

/**
 * loop()
 * This is called repeatedly to allow device to do its work.
 */
void PartOfDay::loop()
{
    unsigned long currentTimeUTC = Time.now();  // seconds since 1/1/1970 UTC
    if(currentTimeUTC >= _lastPollMinuteUTC + 60) // Next minute?
    {
        // Is it the next day?
        _lastPollMinuteUTC = currentTimeUTC;
        if(currentTimeUTC >= _lastPollDayUTC + 24*60*60) {
            _lastPollDayUTC = currentTimeUTC;
            calcSunriseSunset(currentTimeUTC);
        }

        int minutesSinceMidnight = (Time.hour() * 60) + Time.minute();
        int currentPeriod = calcPartOfDay(minutesSinceMidnight);
        if (currentPeriod != _value) {
            _value = currentPeriod;
            publishPOD(_value);
        }
    }
}

void PartOfDay::calcSunriseSunset(unsigned long currentTimeUTC)
{
    computeSun(currentTimeUTC, true);   // Sunrise
    computeSun(currentTimeUTC, false);  // Sunset
}

int PartOfDay::calcPartOfDay(int minutesSinceMidnight)
{
    if (minutesSinceMidnight > _sunsetMinutesAfterMidnight+30) return NIGHT;
    if (minutesSinceMidnight > _sunsetMinutesAfterMidnight) return DUSK;
    if (minutesSinceMidnight == _sunsetMinutesAfterMidnight) return SUNSET;
    if (minutesSinceMidnight > 12*60) return AFTERNOON;
    if (minutesSinceMidnight == 12*60) return NOON;
    if (minutesSinceMidnight > _sunriseMinutesAfterMidnight) return MORNING;
    if (minutesSinceMidnight == _sunriseMinutesAfterMidnight) return SUNRISE;
    if (minutesSinceMidnight > _sunriseMinutesAfterMidnight-30) return DAWN;
    return NIGHT;
}

void PartOfDay::publishPOD(int partOfDay) {
    IoT::publishValue("partofday",partOfDay);
}

// Code From TimeLord
// Replacing 'when' with 0, 0, 12, Time.day(), Time.month(), Time.year()
bool PartOfDay::computeSun(int currentTimeUTC, bool forSunrise) {
    float y, decl, eqt, ha, lon, lat;
    uint8_t a;
  int minutes;
  
  lon = -LONGITUDE / 57.295779513082322;    // Convert to radians
  lat = LATITUDE / 57.295779513082322;      // "
  
  //approximate hour;
  a = forSunrise ? 18 : 6;  // Seems backwards
  
  // approximate day of year
  y= (Time.month()-1) * 30.4375 + (Time.day()-1)  + a/24.0; // 0... 365

  // compute fractional year
  y *= 1.718771839885e-02; // 0... 1

  // compute equation of time... .43068174
  eqt = 229.18 * (0.000075+0.001868*cos(y)  -0.032077*sin(y) -0.014615*cos(y*2) -0.040849*sin(y* 2) );

  // compute solar declination... -0.398272
  decl = 0.006918-0.399912*cos(y)+0.070257*sin(y)-0.006758*cos(y*2)+0.000907*sin(y*2)-0.002697*cos(y*3)+0.00148*sin(y*3);
  
  //compute hour angle
  ha = (cos(1.585340737228125) / (cos(lat)*cos(decl)) -tan(lat) * tan(decl));
  
  if(fabs(ha)>1.0){// we're in the (ant)arctic and there is no rise(or set) today!
      return false;
  }
  
  ha = acos(ha);
  if(forSunrise==false) ha = -ha;
  
  // compute minutes from midnight
  minutes = 720 + 4*(lon-ha)*57.295779513082322-eqt;
  
  // convert from UTC back to our timezone
  minutes += TIMEZONE;
    if(Time.isDST()) {
        minutes += 60;
    }
  
  // adjust the time array by minutes
    
    if(minutes > 1440) {
        Log("POD Error: minutes too big: %d", minutes);
    }
    if(forSunrise) {
        _sunriseMinutesAfterMidnight = minutes;
        Log("POD: sunriseMinutesAfterMidnight =  %d", _sunriseMinutesAfterMidnight);
    } else {
        _sunsetMinutesAfterMidnight = minutes;
        Log("POD: sunsetMinutesAfterMidnight =  %d", _sunsetMinutesAfterMidnight);
    }
    return true;
}
