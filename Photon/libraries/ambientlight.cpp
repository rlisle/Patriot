/******************************************************************
 Ambient Light Level

 Features:
 - Publish periodic light level events

 http://www.github.com/rlisle/rv-arduino-libraries

 Written by Ron Lisle, ron@lisles.net

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2016-11-29: Initial version
 ******************************************************************/

#include "ambientlight.h"

#define defaultInterval 300000
#define defaultText     "Unnamed area"

AmbientLight::AmbientLight(int pin, int minR, int maxR)
{
    this->lastLoopTime = 0;
    this->interval = defaultInterval;
    this->eventText = defaultText;
    this->pin = pin;
    this->minR = minR;
    this->maxR = maxR;
    pinMode(pin, INPUT_PULLUP);
}

void AmbientLight::setInterval(long interval)
{
    this->interval = interval;
}

void AmbientLight::setText(String eventText)
{
  this->eventText = eventText;
}

void AmbientLight::loop() {
  long loopTime = millis();

  if(loopTime > lastLoopTime + interval) {
    // Read photoresistor resistance
    reading = analogRead(pin);
    lastLoopTime = loopTime;

    if (reading != previousReading) {
      level = convertReadingToLevel();
      publishLightLevel();
      previousReading = reading;
    }

  }
}

int AmbientLight::convertReadingToLevel() {
  return (reading - minR) * 100 / (maxR - minR);
}

void AmbientLight::publishLightLevel() {
  String data = eventText + "LightLevel:"+String(level);
  Particle.publish("lislerv", data);
}
