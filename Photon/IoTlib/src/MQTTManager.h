/******************************************************************
MQTTManager.h

This class handles all MQTT interactions.

Note: to avoid making this a singleton, 
the caller must provide the callback handler and forward it to us.

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
#include "behaviors.h"
#include "devices.h"

class MQTTManager
{
public:

  MQTTManager(String publishName, String brokerIP, String connectID, String controllerName, void (*callback)(char*,uint8_t*,unsigned int));

  void        publish(String topic, String message);
  void        loop();
  void        mqttHandler(char* topic, byte* payload, unsigned int length, Devices *devices, Behaviors *behaviors);

private:
    MQTT      *_mqtt;
    String    _publishName;
    String    _connectID;
    String    _brokerIP;
    String    _controllerName;
    void      (*_callback)(char*,uint8_t*,unsigned int);

    void      log(String message);
};
