/******************************************************************
fan control

Features:
- Controls variable speed fan
- Behaviors can be added to vary speed based on temperature, etc.

http://www.github.com/rlisle/ParticleIoT

Written by Ron Lisle, ron@lisles.net

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-05: Convert to v2 particle library
2016-12-08: Convert to IoT model.
2016-07-06: Initial version
******************************************************************/
#include "fan.h"

Fan::Fan(int pinNum, String name)
{
  _behaviors     = new Behaviors();
  _name          = name;
  _pinNum        = pinNum;
  _percent       = 0;
  pinMode(pinNum, OUTPUT);
  setFanSpeed(0);
}

String Fan::name() {
  return _name;
}

void Fan::setPercent(int percent)
{
  setFanSpeed(percent);
}

int Fan::getPercent()
{
  return _percent;
}

// Handle 'On', 'Off', 'Fast', 'Slow', '+', '-', or 0-100
int Fan::convertCommandToPercent(String speed) {

  Serial.print("Set fan ");
  Serial.print(_name);
  Serial.print(" to ");
  Serial.println(speed);

  int percent = 0;
  if(speed.equalsIgnoreCase("on")) {
    percent = 100;

  } else if(speed.equalsIgnoreCase("off")) {
    percent = 0;

  } else if(speed == "+") {
    int increasedPercent = _percent + 10;
    if(increasedPercent > 100) increasedPercent = 100;
    percent = increasedPercent;

  } else if(speed == "-") {
    int decreasedPercent = _percent - 10;
    if(decreasedPercent < 0) decreasedPercent = 0;
    percent = decreasedPercent;

  } else {
    int newPercent = speed.toInt();
    if(newPercent > 100) newPercent = 100;
    percent = newPercent;
  }
  return percent;
}

void Fan::setOn()
{
  setPercent(100);
}

void Fan::setOff()
{
  setPercent(0);
}

bool Fan::isOn()
{
  return(_percent != 0);
}

bool Fan::isOff()
{
  return(_percent == 0);
}

void Fan::loop()
{
  // Nothing to do
}

int Fan::addBehavior(Behavior *behavior)
{
  return _behaviors->addBehavior(behavior);
}

void Fan::performActivities(Activities* activities)
{
    int percent = _behaviors->determineLevelForActivities(0, activities);
    Serial.println("Fan "+_name+" performActivities: setting "+String(percent));
    setFanSpeed(percent);
}

void Fan::setFanSpeed(int percent)
{
  _percent = percent;
  float pwm = _percent * 256.0 / 100.0;
  analogWrite(_pinNum, (int)pwm);
}


/*void publishFanSpeed() {
  String data = "ReferFanSpeed:"+String(int(lround(fanSpeed)), HEX);
  Particle.publish("lislerv", data);
}*/
