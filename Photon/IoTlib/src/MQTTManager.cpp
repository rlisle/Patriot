/******************************************************************
MQTTManager.h

This class handles all MQTT interactions.

Note: to avoid making this a singleton, 
the caller must provide global callback handlers (see externs).

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2018-11-05: Created by refactoring from IoT
******************************************************************/
#include "MQTTManager.h"

extern void globalMQTTHandler(char *topic, byte* payload, unsigned int length);
extern void globalQOScallback(unsigned int);

MQTTManager::MQTTManager(String publishName, String brokerIP, String connectID, String controllerName, MQTTParser *parser)
{
    _publishName = publishName;
    _brokerIP = brokerIP;       // delete?
    _connectID = connectID;     // delete?
    _controllerName = controllerName;
    _parser = parser;

    _mqtt =  new MQTT((char *)brokerIP.c_str(), 1883, globalMQTTHandler);

    _mqtt->connect(_connectID);  
    if (_mqtt->isConnected()) {
        log("MQTT setting QOS callback");
        _mqtt->addQosCallback(globalQOScallback);

        log("MQTT is connected.");
        if(_mqtt->subscribe(publishName+"/#")) {
            log("MQTT subscribed to " + publishName + "/#");
        } else {
            log("Unable to subscribe to MQTT");
        }
    } else {
        log("MQTT is NOT connected! Check MQTT IP address");
    }
}

void MQTTManager::log(String message)
{
    if(_mqtt != NULL && _mqtt->isConnected()) {
        publish("debug/" + _controllerName, message);
    } else {
        Serial.println(message);
    }
}

void MQTTManager::publish(String topic, String message)
{
    _mqtt->publish(topic, message);
}

void MQTTManager::loop()
{
    if(_mqtt != NULL && _mqtt->isConnected()) {
        _mqtt->loop();
    }
}

void MQTTManager::mqttHandler(char* rawTopic, byte* payload, unsigned int length) {

    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = 0;
    String message(p);
    String topic(rawTopic);
    Serial.println("MQTTManager received topic: " + topic + ", message: " + message);

    _parser->parseMessage(topic, message, _mqtt);
}

void MQTTManager::mqttQOSHandler(unsigned int data) {

    log("MQTT QOS callback: " + String(data));
}