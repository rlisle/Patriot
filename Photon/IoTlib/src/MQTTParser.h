/******************************************************************
MQTTParser.h

This class handles all MQTT parsing.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.
******************************************************************/
#pragma once
#include "behaviors.h"
#include "devices.h"
#include "MQTTManager.h"
#include "states.h"

class Devices;
class MQTTManager;

class MQTTParser
{
public:
  States    *_states;

  MQTTParser(String controllerName, Devices *devices);

  void parseMessage(String topic, String message, MQTTManager *mqtt);

private:
  String    _controllerName;
  Devices   *_devices;

  int parseValue(String message);
  void log(String message);

};
