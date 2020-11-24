/******************************************************************
MQTTParser

This class handles all MQTT parsing.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.
******************************************************************/
#include "MQTTParser.h"
#include "constants.h"

MQTTParser::MQTTParser(String controllerName, Devices *devices)
{
    _controllerName = controllerName;
    _devices = devices;
    _states = new States();
}

// topic and messages are already lowercase
void MQTTParser::parseMessage(String topic, String message, MQTT *mqtt)
{
    log("received: " + topic + ", " + message);
    
    // New Protocol: patriot/<name>  <value>
    if(topic.startsWith(kPublishName+"/")) {
        String subtopic = topic.substring(kPublishName.length()+1);
        
        // Look for reserved names
        // PING
        if(subtopic.equals("ping")) {
            // Respond if ping is addressed to us
            if(message.equals(_controllerName)) {
                log("Ping addressed to us");
                mqtt->publish(kPublishName + "/pong", _controllerName);
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
            log("Parser setting state " + subtopic + " to " + message);
            _states->addState(subtopic,percent);
            _devices->stateDidChange(_states);
        }
    }
}

int MQTTParser::parseValue(String message)
{
    if(message.equals("on")) {
        return 100;
    } else if(message.equals("off")) {
        return 0;
    }
    return message.toInt();
}

void MQTTParser::log(String message)
{
    Serial.println("MQTTParser: " + message);
}
