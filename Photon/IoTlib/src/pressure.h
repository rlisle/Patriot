/******************************************************************
 pressure sensor

 Features:
 - pressure
 - altitude

 http://www.github.com/devlware/Patriot

 Written by Diego W. Antunes

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2017-04-15: Initial version
 ******************************************************************/

#pragma once

#include "application.h"
#include "Adafruit_BMP085.h"

class IoT;

class Pressure
{
  friend IoT;

public:
  Pressure();
  void setInterval(long interval);
  void setText(String eventText);

private:
  Adafruit_BMP085 bmp;
  bool      initComplete;

  uint32_t  lastLoopTime;
  String    eventText;
  uint32_t  interval;

  float     pressure;
  float     previousPressure = 0;
  float     temperature;
  float     previousTemperature = 0;
  float     altitude;
  float     previousAltitude = 0;

  float     read_p0(float myAltitude, float ABSpressure);
  void      loop();
  bool      isTimeForNextReading();
  void      processReading();
  void      handleReadingData();

  void      publishPressureIfValid();
  void      publishAltitudeIfValid();
  void      publishTemperatureIfValid();

  bool      isValidTemperatureReading();
  bool      isValidAltitudeReading();
  bool      isValidPressureReading();

  void      publishTemperature();
  void      publishAltitude();
  void      publishPressure();
};
