/******************************************************************
Temperature

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
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle library
2016-11-28: Initial version
******************************************************************/
#include "temperature.h"
#include "math.h"         // Needed for lround()

#define kDefaultInterval 15000
#define kDefaultText     "Unnamed area"

// These constants define the expected reasonable values.
// Readings outside of these values will be ignored.
#define kMaxTemp         120
#define kMinTemp         20
#define kMaxHumidity     100
#define kMinHumidity     10

Temperature::Temperature(int pin, int type)
{
    this->lastLoopTime = 0;
    this->interval = kDefaultInterval;
    this->eventText = kDefaultText;

    dht = new PietteTech_DHT(pin, type);
}

void Temperature::setInterval(long interval)
{
    this->interval = interval;
}

void Temperature::setText(String eventText)
{
  this->eventText = eventText;
}

void Temperature::loop() {

  if(isTimeForNextReading()) {

    if (!hasReadingStarted) {
      Serial.print("Retrieving information from sensor: ");
      dht->acquire();
      hasReadingStarted = true;
    }

    if (hasReadingCompleted()) {
      processReading();
      hasReadingStarted = false;
    }

    lastLoopTime = millis();
  }
}

bool Temperature::hasReadingCompleted() {
  return( !dht->acquiring() );
}

bool Temperature::isTimeForNextReading() {
  return (millis() > lastLoopTime + interval);
}

void Temperature::processReading() {
  int result = dht->getStatus();
  switch (result) {
    case DHTLIB_OK:
      handleReadingData();
      break;
    case DHTLIB_ERROR_CHECKSUM:
      Serial.println("Error: Checksum error");
      break;
    case DHTLIB_ERROR_ISR_TIMEOUT:
      Serial.println("Error: ISR time out error");
      break;
    case DHTLIB_ERROR_RESPONSE_TIMEOUT:
      Serial.println("Error: Response time out error");
      break;
    case DHTLIB_ERROR_DATA_TIMEOUT:
      Serial.println("Error: Data time out error");
      break;
    case DHTLIB_ERROR_ACQUIRING:
      Serial.println("Error: Acquiring");
      break;
    case DHTLIB_ERROR_DELTA:
      Serial.println("Error: Delta time to small");
      break;
    case DHTLIB_ERROR_NOTSTARTED:
      Serial.println("Error: tNot started");
      break;
    default:
      Serial.println("Error: unknown");
      break;
  }
}

void Temperature::handleReadingData() {
  temperature = int(lround(dht->getFahrenheit()));
  humidity = int(lround(dht->getHumidity()));
  Serial.println("OK. Temp: "+String(temperature)+", Humidity: "+humidity);

  publishTemperatureIfValid();
  publishHumidityIfValid();
}

void Temperature::publishHumidityIfValid() {
  if (isValidHumidityReading()) {
    if(humidity != previousHumidity) {
      publishHumidity();
      previousHumidity = humidity;
    }
  } else {
    Serial.println("Humidity reading is invalid: "+String(humidity));
  }
}

void Temperature::publishTemperatureIfValid() {
  if (isValidTemperatureReading()) {
    if(temperature != previousTemperature){
      publishTemperature();
      previousTemperature = temperature;
    }
  } else {
    Serial.println("Temperature reading is invalid: "+String(temperature));
  }
}

bool Temperature::isValidTemperatureReading() {
  return (temperature >= kMinTemp && temperature <= kMaxTemp);
}

bool Temperature::isValidHumidityReading() {
  return (humidity >= kMinHumidity && humidity <= kMaxHumidity);
}

void Temperature::publishTemperature() {
  String data = eventText + "Temp:"+String(temperature);
  //TODO: get event name from IoT instead of hardcoding "patriot"
  Particle.publish("patriot", data);
}

void Temperature::publishHumidity() {
  String data = eventText + "Humidity:"+String(humidity);
  //TODO: get event name from IoT instead of hardcoding "patriot"
  Particle.publish("patriot", data);
}
