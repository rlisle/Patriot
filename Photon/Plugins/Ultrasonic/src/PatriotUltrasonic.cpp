/******************************************************************
PatriotUltrasonic plugin

Features:
- Reports distance to nearest object in inches

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2017-05-17: Move to separate library
2017-05-15: Make devices generic
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle library
2016-07-29: Refactor to separate switch and switches classes
         Add mapping to an activity
2016-06-21: Photon version
2016-02-3: Initial version
******************************************************************/

#include "PatriotUltrasonic.h"

Ultrasonic::Ultrasonic(int triggerPin, int echoPin, String name)
{
    _triggerPin         = triggerPin;
    _echoPin            = echoPin;
    _name               = name;
    _thresholdInInches  = kThresholdInInches;

    distanceInInches    = 0;

    pinMode(_triggerPin, OUTPUT);
    digitalWrite(_triggerPin, LOW);
    if(_triggerPin != _echoPin) {
        pinMode(_echoPin, INPUT_PULLUP);
    }
}


void Ultrasonic::loop()
{
    long time = millis();
    if (isTimeToPing(time))
    {
        _lastPingTime = time;
        int newDistance = ping();
        if (didDistanceChange(newDistance))
        {
            distanceInInches = newDistance;
            notify(newDistance);
        }
    }
}


bool Ultrasonic::isTimeToPing(long time)
{
    return (time > _lastPingTime + kPingInterval);
}


bool Ultrasonic::didDistanceChange(int newDistance)
{
    return ((newDistance > distanceInInches + _thresholdInInches)
        || (newDistance < distanceInInches - _thresholdInInches));
}


int Ultrasonic::ping()
{
    // establish variables for duration of the ping,
    // and the distance result in inches and centimeters:
    long duration;
    int  inches;

    // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
    // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
    // If using the same pin, need to set to output and low state
    if(_triggerPin == _echoPin){
        pinMode(_triggerPin, OUTPUT);
        digitalWrite(_triggerPin, LOW);
        delayMicroseconds(5);
    }
    digitalWrite(_triggerPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(_triggerPin, LOW);

    // The same pin can be used to read the signal from the PING))): a HIGH
    // pulse whose duration is the time (in microseconds) from the sending
    // of the ping to the reception of its echo off of an object.
    if(_echoPin == _triggerPin) {
        pinMode(_echoPin, INPUT_PULLUP);
    }
    duration = pulseIn(_echoPin, HIGH);
//  Serial.println("Ping: duration = "+String(duration));
    if(_echoPin == _triggerPin) {
        pinMode(_triggerPin, OUTPUT);
        digitalWrite(_triggerPin, LOW);
    }

    // convert the time into a distance
    inches = microsecondsToInches(duration);
    //cm = microsecondsToCentimeters(duration);

    return inches;
}

// Private
int Ultrasonic::microsecondsToInches(long microseconds) {
    // According to Parallax's datasheet for the PING))), there are
    // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
    // second).  This gives the distance travelled by the ping, outbound
    // and return, so we divide by 2 to get the distance of the obstacle.
    // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
    return microseconds / 74 / 2;
}

int Ultrasonic::microsecondsToCentimeters(long microseconds) {
    // The speed of sound is 340 m/s or 29 microseconds per centimeter.
    // The ping travels out and back, so to find the distance of the
    // object we take half of the distance travelled.
    return microseconds / 29 / 2;
}

void Ultrasonic::notify(int distance)
{
    String pubString = _name + ":" + String(distance);
    Serial.println(pubString);
    //TODO: get event name from IoT instead of hardcoded "patriot"
    Particle.publish("patriot", pubString);
}
