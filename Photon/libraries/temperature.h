/******************************************************************
 temperature and humidity

 Features:
 - Periodically publishes temperature and humidity information

 http://www.github.com/rlisle/rv-arduino-libraries

 Written by Ron Lisle, ron@lisles.net

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2016-11-28: Initial version
 ******************************************************************/

#ifndef temperature_h
#define temperature_h

#include "application.h"
#include "DHT.h"

class IoT;

class Temperature
{
  friend IoT;

public:
  Temperature(int pin, int type);
  void setInterval(long interval);
  void setText(String eventText);

private:
  PietteTech_DHT *dht;
  uint32_t  lastLoopTime;
  String    eventText;
  uint32_t  interval;

  int       humidity;
  int       previousHumidity    = 0;
  int       temperature;
  int       previousTemperature = 0;
  bool      hasReadingStarted   = false;

  void      loop();
  bool      isTimeForNextReading();
  bool      hasReadingCompleted();
  void      processReading();
  void      handleReadingData();
  void      publishTemperatureIfValid();
  void      publishHumidityIfValid();
  bool      isValidTemperatureReading();
  bool      isValidHumidityReading();
  void      publishTemperature();
  void      publishHumidity();

};

#endif /* temperature_h */
