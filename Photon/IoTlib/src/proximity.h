/******************************************************************
Ultrasonic Proxmity sensor

Features:
- Reports distance to nearest object in inches

http://www.github.com/rlisle/ParticleIoT

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-05: Convert to v2 particle library
2016-08-14: Initial version copied from Ping Arduino example
******************************************************************/
#pragma once

#define kPingInterval        500   // 1/2 second

class Proximity
{
private:

  int     _trigPin;
  int     _echoPin;

  int microsecondsToInches(long microseconds);
  int microsecondsToCentimeters(long microseconds);

public:

  int     inches;

  Proximity(int trigPin, int echoPin);

  int     ping();

};
