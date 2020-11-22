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

void MQTTParser::parseMessage(String topic, String message, MQTT *mqtt)
{
    log("received: " + topic + ", " + message);
    
    if(topic.startsWith(kPublishName)) {
        // LEGACY
        if(topic.length() == kPublishName.length()) {
            int colonPosition = message.indexOf(':');
            String name = message.substring(0,colonPosition);
            String state = message.substring(colonPosition+1);
            // See if this is a device name. If so, update it.
            Device* device = _devices->getDeviceWithName(name);
            if(device)
            {
                int percent = parseValue(state);
                device->setPercent(percent);
                return;
            }
            // If it wasn't a device name, it must be a state.
            int value = state.toInt();
            _states->addState(name,value);
            _devices->stateDidChange(_states);
            
        } else {
            // New Protocol: patriot/<name>  <value>
            // There should be only 1 slash
            int firstSlash = topic.indexOf('/');
            int lastSlash = topic.lastIndexOf('/');
            if(firstSlash == -1 || firstSlash != lastSlash) {
                log("Message does not contain 1 slash, so invalid. Ignoring");
                return;
            }
            String rightTopic = topic.substring(firstSlash+1).toLowerCase();
            
            // Look for reserved names
            // PING
            if(rightTopic.equals("ping")) {
                // Respond if ping is addressed to us
                if(message.equalsIgnoreCase(_controllerName)) {
                    log("Ping addressed to us");
                    mqtt->publish(kPublishName + "/pong/" + _controllerName, message);
                }
                
                // PONG
            } else if(rightTopic.equals("pong")) {
                // Ignore it.
                
                // RESET
            } else if(rightTopic.equals("reset")) {
                // Respond if reset is addressed to us
                if(message.equalsIgnoreCase(_controllerName)) {
                    log("Reset addressed to us");
                    System.reset();
                }
                
                // MEMORY
            } else if(rightTopic.equals("memory")) {
                // Respond if memory is addressed to us
                if(message.equalsIgnoreCase(_controllerName)) {
                    log("Memory addressed to us");
                    log( String::format("Free memory = %d", System.freeMemory()));
                }
                
            } else if(rightTopic.equals("log")) {
                // Ignore it.
                
                // UNKNOWN
            } else {
                
                int percent = parseValue(message);
                
                Device* device = _devices->getDeviceWithName(rightTopic);
                if(device)
                {
//                    log("Device " + rightTopic + " found, setting to " + message);
                    device->setPercent(percent);
                    // Fall thru to add to state table also, in case is used in a condition
                }
                _states->addState(rightTopic,percent);
                _devices->stateDidChange(_states);
            }
        }
    }
}

int MQTTParser::parseValue(String message)
{
    if(message.equalsIgnoreCase("on")) {
        return 100;
    } else if(message.equalsIgnoreCase("off")) {
        return 0;
    }
    return message.toInt();
}

void MQTTParser::log(String message)
{
    Serial.println("MQTTParser: " + message);
}
