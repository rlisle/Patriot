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
2018-01-18: Add name and type properties
2017-05-20: Convert to plugin
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle library
2016-11-28: Initial version
******************************************************************/

#include "PatriotDHT.h"
#include "math.h"         // Needed for lround()

extern String publishNameVariable;

#define kDefaultInterval 15000              // Every 15 seconds
#define kDefaultText     "Unnamed"

// These constants define the expected reasonable values.
// Readings outside of these values will be ignored.
#define kMaxTemp         120
#define kMinTemp         20
#define kMaxHumidity     100
#define kMinHumidity     10

/**
 * Constructor.
 */
DHT::DHT(int pin, String name, int type)
    : Device(name, DeviceType::TempHumidity)
{
    lastLoopTime = 0;
    interval = kDefaultInterval;
    eventText = kDefaultText;

    dht = new PietteTech_DHT(pin, type);
}

void DHT::setInterval(long interval)
{
    this->interval = interval;
}

void DHT::setText(String eventText)
{
    this->eventText = eventText;
}

void DHT::loop() {

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

bool DHT::hasReadingCompleted() {
    return( !dht->acquiring() );
}

bool DHT::isTimeForNextReading() {
    return (millis() > lastLoopTime + interval);
}

void DHT::processReading() {
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

void DHT::handleReadingData() {
    temperature = int(lround(dht->getFahrenheit()));
    humidity = int(lround(dht->getHumidity()));
    Serial.println("OK. Temp: "+String(temperature)+", Humidity: "+humidity);

    publishTemperatureIfValid();
    publishHumidityIfValid();
}

void DHT::publishHumidityIfValid() {
    if (isValidHumidityReading()) {
        if(humidity != previousHumidity) {
            publishHumidity();
            previousHumidity = humidity;
        }
    } else {
        Serial.println("Humidity reading is invalid: "+String(humidity));
    }
}

void DHT::publishTemperatureIfValid() {
    if (isValidTemperatureReading()) {
        if(temperature != previousTemperature){
            publishTemperature();
            previousTemperature = temperature;
        }
    } else {
        Serial.println("Temperature reading is invalid: "+String(temperature));
    }
}

bool DHT::isValidTemperatureReading() {
    return (temperature >= kMinTemp && temperature <= kMaxTemp);
}

bool DHT::isValidHumidityReading() {
    return (humidity >= kMinHumidity && humidity <= kMaxHumidity);
}

void DHT::publishTemperature() {
    String data = eventText + "Temp:"+String(temperature);
    Particle.publish(publishNameVariable, data, 60, PRIVATE);
}

void DHT::publishHumidity() {
    String data = eventText + "Humidity:"+String(humidity);
    Particle.publish(publishNameVariable, data, 60, PRIVATE);
}
