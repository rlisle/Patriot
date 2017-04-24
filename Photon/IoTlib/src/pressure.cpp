/******************************************************************
Pressure sensor

Features:
- Publish periodic temperature, pressure and altitude events.

http://www.github.com/devlware/Patriot

Written by Diego W. Antunes

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2017-04-15: Initial version
******************************************************************/
#include "pressure.h"
#include "math.h"         // Needed for lround()

#define kDefaultInterval 15000
#define kDefaultText     "Unnamed area"

// These constants define the expected reasonable values.
// Readings outside of these values will be ignored.
const int kMaxTemp     = 85;    // In Celsius
const int kMinTemp     = -40;   // In Celsius
const int kMaxAltitude = 9000;  // In meters
const int kMinAltitude = -500;  // In meters
const int kMaxPressure = 1100;  // In hPa
const int kMinPressure = 300;   // In hPa

Pressure::Pressure()
{
    this->lastLoopTime = 0;
    this->interval = kDefaultInterval;
    this->eventText = kDefaultText;

    if (!bmp.begin()) {
        Serial.println("Could not find a valid BMP085 sensor, check wiring!");
        initComplete = false;
    } else {
        initComplete = true;
    }
}

float Pressure::read_p0(float myAltitude, float ABSpressure)
{
    float p0 = ABSpressure / pow((1.0 - ( myAltitude / 44330.0 )), 5.255);
    return p0;
}

void Pressure::setInterval(long interval)
{
    this->interval = interval;
}

void Pressure::setText(String eventText)
{
    this->eventText = eventText;
}

void Pressure::loop()
{
    if (isTimeForNextReading()) {
        Serial.println("Retrieving information from sensor: ");
        processReading();
        lastLoopTime = millis();
    }
}

void Pressure::processReading()
{
    temperature = bmp.readTemperature();
    Serial.print("Temperature = ");
    Serial.print(temperature);
    Serial.println(" *C");

    pressure = bmp.readPressure() / 100.0f;
    Serial.print("Pressure = ");
    Serial.print(pressure);
    Serial.println(" Pa");

    float p0 = read_p0(951, pressure);
    Serial.print("P0 = ");
    Serial.print(p0);
    Serial.println(" Pa");

    // Calculate altitude assuming 'standard' barometric
    // pressure of 1013.25 millibar = 101325 Pascal
    altitude = bmp.readAltitude();
    Serial.print("Altitude = ");
    Serial.print(bmp.readAltitude());
    Serial.println(" meters");

    // you can get a more precise measurement of altitude
    // if you know the current sea level pressure which will
    // vary with weather and such. If it is 1015 millibars
    // that is equal to 101500 Pascals.

    // Serial.print("Real altitude = ");
    // Serial.print(bmp.readAltitude(p0 * 100));
    // Serial.println(" meters");

    publishTemperatureIfValid();
    publishPressureIfValid();
    publishAltitudeIfValid();
}

bool Pressure::isTimeForNextReading()
{
    return (millis() > lastLoopTime + interval);
}

void Pressure::publishTemperature()
{
    char szEventInfo[64];
    sprintf(szEventInfo, "%s:Temp:%s", String(eventText).c_str(), String(temperature, 2).c_str());
    //TODO: get event name from IoT instead of hardcoding "patriot"
    Particle.publish("patriot", szEventInfo);
}

void Pressure::publishAltitude()
{
    char szEventInfo[64];
    sprintf(szEventInfo, "%s:Altitude:%s", String(eventText).c_str(), String(altitude, 2).c_str());
    //TODO: get event name from IoT instead of hardcoding "patriot"
    Particle.publish("patriot", szEventInfo);
}

void Pressure::publishPressure()
{
    char szEventInfo[64];
    sprintf(szEventInfo, "%s:Pressure:%s", String(eventText).c_str(), String(pressure, 2).c_str());
    //TODO: get event name from IoT instead of hardcoding "patriot"
    Particle.publish("patriot", szEventInfo);
}

bool Pressure::isValidTemperatureReading()
{
    return (temperature >= kMinTemp && temperature <= kMaxTemp);
}

bool Pressure::isValidAltitudeReading()
{
    return (altitude >= kMinAltitude && altitude <= kMaxAltitude);
}

bool Pressure::isValidPressureReading()
{
    return (pressure >= kMinPressure && pressure <= kMaxPressure);
}

void Pressure::publishPressureIfValid()
{
    if (isValidPressureReading()) {
       if (pressure != previousPressure) {
            publishPressure();
            previousPressure = pressure;
        }
    } else {
        Serial.println("Pressure reading is invalid: " + String(pressure));
    }
}

void Pressure::publishAltitudeIfValid()
{
    if (isValidAltitudeReading()) {
       if (altitude != previousAltitude) {
            publishAltitude();
            previousAltitude = altitude;
        }
    } else {
        Serial.println("Altitude reading is invalid: " + String(altitude));
    }
}

void Pressure::publishTemperatureIfValid()
{
    if (isValidTemperatureReading()) {
       if (temperature != previousTemperature) {
            publishTemperature();
            previousTemperature = temperature;
        }
    } else {
        Serial.println("Temperature reading is invalid: " + String(temperature));
    }
}
