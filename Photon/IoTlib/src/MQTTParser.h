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
#include "MQTTManager.h"

class MQTTParser
{
public:

  MQTTParser();


private:
    MQTTManager   *_mqtt;
};
