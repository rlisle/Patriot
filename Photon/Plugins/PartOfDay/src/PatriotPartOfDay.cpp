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
}

/**
 begin is called after publishPtr is set, so we can publish here but not in constructor
 */
void PartOfDay::begin() {
    // Force next loop to perform both
    _lastPollDay = 0;
    _lastPollMinute = 0;
}

/**
 * loop()
 * This is called repeatedly to allow device to do its work.
 */
void PartOfDay::loop()
{
    unsigned long currentTimeUTC = now();  // seconds since 1/1/1970 UTC
    if(currentTimeUTC >= _lastPollMinute + 60) // Next minute?
    {
        _lastPollMinuteUTC = currentTimeUTC;
        if( Time.minute() % 15 == 0 ) {
            Log("The time now is %d:%d",Time.hour(),Time.minute());
        }

        if(currentTimeUTC >= _lastPollDayUTC + 24*60*60) {    // Next day?
            _lastPollDayUTC = currentTimeUTC;
            calcSunriseSunset(currentTimeUTC);
        }

        int minutesSinceMidnight = (Time.hour() * 60) + Time.minute();
        int currentPeriod = calcPartOfDay(minutesSinceMidnight);
        if (currentPeriod != _value) {
            _value = currentPeriod;
            Log.info("PartOfDay changed to %d", _value);
            publishPOD(_value);
        }
    }
}

void PartOfDay::calcSunriseSunset(unsigned long currentTimeUTC)
{
    ComputeSun(currentTimeUTC, true);   // Sunrise
    ComputeSun(currentTimeUTC, false);  // Sunset
//    Log.info("Sunrise today %d/%d is %d:%d",Time.month(), Time.day(), _periods[0].hour, _periods[0].minute);
//    Log.info("Sunset today %d/%d is %d:%d",Time.month(), Time.day(), _periods[4].hour, _periods[4].minute);

//    _periods[SUNRISE-1].set(sunriseHour, sunriseMinute);
//    _periods[MORNING-1].set(sunriseHour, sunriseMinute+1);
//    _periods[NOON-1].set(12,0);
//    _periods[AFTERNOON-1].set(12,1);
//    _periods[SUNSET-1].set(sunsetHour, sunsetMinute);
//    _periods[DUSK-1].set(sunsetHour, sunsetMinute+1);
//    _periods[NIGHT-1].set(sunsetHour, sunsetMinute+30);
//    _periods[DAWN-1].set(sunriseHour, sunriseMinute - 30);
}

int PartOfDay::calcPartOfDay(int minutesSinceMidnight)
{
//    Period current(Time.hour(),Time.minute());
    
    if (minutesSinceMidnight > sunsetMinutesAfterMidnight+30) return NIGHT;
    if (minutesSinceMidnight > sunsetMinutesAfterMidnight) return DUSK;
    if (minutesSinceMidnight == sunsetMinutesAfterMidnight) return SUNSET;
    if (minutesSinceMidnight > 12*60) return AFTERNOON;
    if (minutesSinceMidnight == 12*60) return NOON;
    if (minutesSinceMidnight > sunriseMinutesAfterMidnight) return MORNING;
    if (minutesSinceMidnight == sunriseMinutesAfterMidnight) return SUNRISE;
    if (minutesSinceMidnight > sunriseMinutesAfterMidnight-30) return DAWN;
    return NIGHT;
}

void PartOfDay::publishPOD(int partOfDay) {
    IoT::publishValue("partofday",partOfDay);
}

// Code From TimeLord
// Replacing 'when' with 0, 0, 12, Time.day(), Time.month(), Time.year()
bool PartOfDay::ComputeSun(int currentTimeUTC, bool forSunrise) {
  float y, decl, eqt, ha, lon, lat, z;
    uint8_t a;
  int doy, minutes;
  
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
  
  // adjust the time array by minutes
    
//  when[tl_hour]=0;
//  when[tl_minute]=0;
//  when[tl_second]=0;
  Adjust(minutes, forSunrise);
    return true;
}

// This will adjust the day of sunrise/sunset if it crosses over midnight, etc.
void PartOfDay::Adjust(long offset, bool forSunrise){
    long tmp, mod, nxt;
    
    tmp=offset;
    nxt=tmp/60;                // hours
    mod=Absolute(tmp) % 60;
    mod=mod*Signum(tmp)+60;
    mod %= 60;
    if(forSunrise) {
        sunrise.minute = mod;
    } else {
        sunset.minute = mod;
    }
    
    tmp=nxt;
    nxt=tmp/24;                    // days
    mod=Absolute(tmp) % 24;
    mod=mod*Signum(tmp)+24;
    mod %= 24;
    if(forSunrise) {
        sunrise.hour = mod;
    } else {
        sunset.hour = mod;
    }

//    tmp=nxt+_day;
//    mod=LengthOfMonth(_month);
//
//    _day = tmp;
//    if(tmp>mod){
//            while (tmp>mod) {
//        tmp -= mod;
//        when[tl_month]++;
//                if (when[tl_month] > 12) {
//                    when[tl_month] = 1;
//                    when[tl_year]++;
//                    when[tl_year] += 100;
//                    when[tl_year] %= 100;
//                }
//                mod = LengthOfMonth(when);
//            }
//            when[tl_day]=tmp;
//    } else if (tmp<1) {
//            while (tmp<1) {
//        when[tl_month]--;
//                if (when[tl_month] < 1) {
//                    when[tl_month] = 12;
//                    when[tl_year] --;
//                    when[tl_year] += 100;
//                    when[tl_year] %= 100;
//                }
//        mod=LengthOfMonth(when);
//        when[tl_day]=tmp+mod;
//                tmp += mod;
//            }
//    }
}

int PartOfDay::Absolute(int n){
    if(n<0) return 0-n;
    return n;
}

