/******************************************************************
PatriotUltrasonic plugin

Features:
- Reports distance to nearest object in inches

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2017-05-17: Move to separate library
2017-05-15: Make devices generic
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle library
2016-07-29: Refactor to separate switch and switches classes
         Add mapping to an activity
2016-06-21: Photon version
2016-02-3: Initial version
******************************************************************/
#pragma once

#include "Particle.h"
#include "device.h"

#define kPingInterval       500 // 1/2 second between measurements
#define kThresholdInInches  3   // Amount of change before notify sent

class Ultrasonic : public Device
{
public:
    int     distanceInInches;

    Ultrasonic(int triggerPin, int echoPin, String name);
    void    loop();

private:
    int     _triggerPin;
    int     _echoPin;
    String  _name;
    long    _lastPingTime;
    int     _thresholdInInches;

    bool    isTimeToPing(long time);
    int     ping();
    bool    didDistanceChange(int distance);
    int     microsecondsToInches(long microseconds);
    int     microsecondsToCentimeters(long microseconds);
    void    notify(int distance);
};
