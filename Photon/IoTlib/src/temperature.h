/******************************************************************
temperature and humidity

Features:
- Periodically publishes temperature and humidity information

http://www.github.com/rlisle/ParticleIoT

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-05: Convert to v2 particle library
2016-11-28: Initial version
******************************************************************/
#pragma once

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
