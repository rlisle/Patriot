/******************************************************************
Ultrasonic Proxmity sensor

Features:
- Reports distance to nearest object in inches
- Trigger pin and Echo pin can be the same pin, or separate

http://www.github.com/rlisle/ParticleIoT

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-05: Convert to v2 particle library
2016-08-14: Initial version copied from Ping Arduino example
******************************************************************/
#include "application.h"
#include "proximity.h"

Proximity::Proximity(int trigPin, int echoPin)
{
  _trigPin                  = trigPin;
  _echoPin                  = echoPin;
  pinMode(_trigPin, OUTPUT);
  digitalWrite(_trigPin, LOW);
  if(_trigPin != _echoPin) {
    pinMode(_echoPin, INPUT_PULLUP);
  }
}

int Proximity::ping()
{
  // establish variables for duration of the ping,
  // and the distance result in inches and centimeters:
  long duration;
  int  inches;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  // If using the same pin, need to set to output and low state
  if(_trigPin == _echoPin){
    pinMode(_trigPin, OUTPUT);
    digitalWrite(_trigPin, LOW);
    delayMicroseconds(5);
  }
  digitalWrite(_trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(_trigPin, LOW);

  // The same pin can be used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  if(_echoPin == _trigPin) {
    pinMode(_echoPin, INPUT_PULLUP);
  }
  duration = pulseIn(_echoPin, HIGH);
//  Serial.println("Ping: duration = "+String(duration));
  if(_echoPin == _trigPin) {
    pinMode(_trigPin, OUTPUT);
    digitalWrite(_trigPin, LOW);
  }

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  //cm = microsecondsToCentimeters(duration);

  return inches;
}

// Private
int Proximity::microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

int Proximity::microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
