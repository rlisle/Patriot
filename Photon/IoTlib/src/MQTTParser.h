/******************************************************************
MQTTParser.h

This class handles all MQTT parsing.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2018-11-07: Created by refactoring from IoT
******************************************************************/
#pragma once
#include "behaviors.h"
#include "devices.h"
#include "MQTT.h"

class MQTTParser
{
public:

  MQTTParser(String controllerName, String publishName, Devices *devices, Behaviors *behaviors);

  void parseMessage(String topic, String message, MQTT *mqtt);

private:
  String    _controllerName;
  String    _publishName;
  Devices   *_devices;
  Behaviors *_behaviors;
};
