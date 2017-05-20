/******************************************************************
PatriotDHT

Features:
- Publish periodic temperature and humidity events

Note: temperature and humidity considered valid within limits
     normal for indoors. Change constants if used outdoors in
     extreme climates.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2017-05-20: Convert to plugin
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle library
2016-11-28: Initial version
******************************************************************/
#pragma once

#include "Particle.h"
#include "DHT.h"
#include "device.h"

class DHT : public Device
{
 public:
    DHT(int pin, int type);
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
