/******************************************************************
  Ultrasonic Proxmity sensor

 Features:
 - Reports distance to nearest object in inches

 http://www.github.com/rlisle/rv-arduino-libraries

 Written by Ron Lisle, ron@lisles.net

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2016-08-14: Initial version copied from Ping Arduino example
 ******************************************************************/

#ifndef proximity_h
#define proximity_h

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

#endif /* motion_h */
