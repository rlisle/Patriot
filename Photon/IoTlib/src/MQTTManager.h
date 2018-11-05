/******************************************************************
MQTTManager.h

This class handles all MQTT interactions.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2018-11-05: Created by refactoring from IoT
******************************************************************/
#pragma once

#include "Particle.h"
#include "MQTT.h"

class MQTTManager
{
public:

  MQTTManager(String publishName);

  void        loop();

  void mqttHandler(char* topic, byte* payload, unsigned int length);

private:
    MQTT      *_mqtt;
    String    _publishName;
    String    _connectID;


};
