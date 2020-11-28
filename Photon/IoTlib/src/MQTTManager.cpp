/******************************************************************
MQTTManager.cpp

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
#include "constants.h"

extern void globalMQTTHandler(char *topic, byte* payload, unsigned int length);

MQTTManager::MQTTManager(String brokerIP, String connectID, String controllerName, Devices *devices)
{
    _controllerName = controllerName;
    _devices = devices;
    
    _states = new States();

    _mqtt =  new MQTT((char *)brokerIP.c_str(), 1883, globalMQTTHandler);
    connect(connectID);
}

void MQTTManager::connect(String connectID) {

    _lastMQTTtime = Time.now();

    if(_mqtt == NULL) {
        log("ERROR! MQTTManager: connect called but object null");
    }

    if(_mqtt->isConnected()) {
        log("MQTT is connected, so reconnecting...");
        _mqtt->disconnect();
    }

    _mqtt->connect(connectID);
    if (_mqtt->isConnected()) {
        if(_mqtt->subscribe(kPublishName+"/#") == false) {
            log("Unable to subscribe to MQTT " + kPublishName + "/#");
        }
    } else {
        log("MQTT is NOT connected! Check MQTT IP address");
    }
    log("Connected at " + String(_lastMQTTtime));
}

void MQTTManager::log(String message)
{
    if(!publish("debug/" + _controllerName, message)){
        Serial.println(_controllerName + " MQTT log: " + message);
    }
}

bool MQTTManager::publish(String topic, String message) {
    if(_mqtt != NULL && _mqtt->isConnected()) {
        Serial.println("Publishing "+String(topic)+" "+String(message));
        _mqtt->publish(topic,message);
        return true;
    }
    return false;
}

void MQTTManager::loop()
{
    if(_mqtt != NULL && _mqtt->isConnected()) {
        _mqtt->loop();
    }

    reconnectCheck();
}

void MQTTManager::reconnectCheck() {
//    system_tick_t secondsSinceLastMessage = Time.now() - _lastMQTTtime;
//    if(secondsSinceLastMessage > 5 * 60) {
//        log("WARNING: connection lost, reconnecting. _lastMQTTtime = " + String(_lastMQTTtime) + ", Time.now() = " + String(Time.now()));
//        connect();
//    }
}

void MQTTManager::mqttHandler(char* rawTopic, byte* payload, unsigned int length) {

    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = 0;
    String message(p);
    String topic(rawTopic);
    //log("received t: " + topic + ", m: " + message);

    _lastMQTTtime = Time.now();

    parseMessage(topic.toLowerCase(), message.toLowerCase());
}

//Mark - Parser

// topic and messages are already lowercase
void MQTTManager::parseMessage(String topic, String message)
{
    //log("Parser received: " + topic + ", " + message);
    
    // New Protocol: patriot/<name>  <value>
    if(topic.startsWith(kPublishName+"/")) {
        //log("parsing t:" + topic + ", m:" + message);
        String subtopic = topic.substring(kPublishName.length()+1);
        
        // Look for reserved names
        // PING
        if(subtopic.equals("ping/"+)) {
            // Respond if ping is addressed to us
            if(message.equals(_controllerName)) {
                log("Ping addressed to us");
                _mqtt->publish(kPublishName + "/pong", _controllerName);
            }
            
            // PONG
        } else if(subtopic.equals("pong")) {
            // Ignore it.
            
            // RESET
        } else if(subtopic.equals("reset")) {
            // Respond if reset is addressed to us
            if(message.equals(_controllerName)) {
                log("Reset addressed to us");
                System.reset();
            }
            
            // MEMORY
        } else if(subtopic.equals("memory")) {
            // Respond if memory is addressed to us
            if(message.equals(_controllerName)) {
                log("Memory addressed to us");
                log( String::format("Free memory = %d", System.freeMemory()));
            }
            
        } else if(subtopic.equals("log")) {
            // Ignore it.
            
            // UNKNOWN
        } else {
            
            int percent = parseValue(message);
            //log("Parser setting state " + subtopic + " to " + message);
            _states->addState(subtopic,percent);
            _devices->stateDidChange(_states);
        }
    } else {
        log("  Not our message");
    }
}

int MQTTManager::parseValue(String message)
{
    if(message.equals("on")) {
        return 100;
    } else if(message.equals("off")) {
        return 0;
    }
    return message.toInt();
}

