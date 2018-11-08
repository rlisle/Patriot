/******************************************************************
MQTTParser

This class handles all MQTT parsing.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2018-11-07: Created by refactoring from IoT
******************************************************************/
#include "MQTTParser.h"

MQTTParser::MQTTParser(String publishName, Devices *devices, Behaviors *behaviors)
{
    _publishName = publishName;
    _devices = devices;
    _behaviors = behaviors;
}

MQTTParser::parseMessage(String topic, String message)
{
    uint publishNameLength = _publishName.length();

    log("MQTT received: " + topic + ", " + data);

    if(topic.startsWith(_publishName)) {
        if(topic.length() == publishNameLength) {   // legacy
            int colonPosition = data.indexOf(':');
            String name = data.substring(0,colonPosition);
            String state = data.substring(colonPosition+1);
            // See if this is a device name. If so, update it.
            Device* device = devices->getDeviceWithName(name);
            if(device)
            {
                int percent = state.toInt();
                device->setPercent(percent);
                return;
            }
            // If it wasn't a device name, it must be an activity.
            int value = state.toInt();
            behaviors->performActivity(name, value);

        } else {
            int firstSlash = topic.indexOf('/');
            int lastSlash = topic.lastIndexOf('/');
            if(firstSlash == -1 || lastSlash == -1 || firstSlash == lastSlash) {
                log("MQTT message does not contain 2 slashes, so ignoring");
                return;
            }
            String midTopic = topic.substring(firstSlash+1,lastSlash);
            String rightTopic = topic.substring(lastSlash+1);
            // Handle various topic messages
            // DEVICE
            if(midTopic.equalsIgnoreCase("device")) {
                Device* device = devices->getDeviceWithName(rightTopic);
                if(device)
                {
                    int percent = data.toInt();
                    device->setPercent(percent);
                }

            // ACTIVITY
            } else if(midTopic.equalsIgnoreCase("activity")) {
                int value = data.toInt();
                behaviors->performActivity(rightTopic, value);

            // PING
            } else if(midTopic.equalsIgnoreCase("ping")) {
                // Respond if ping is addressed to us
                if(rightTopic.equalsIgnoreCase(_controllerName)) {
                    log("Ping addressed to us: "+_controllerName);
                    _mqtt->publish(_publishName + "/pong/" + _controllerName, data);
                }

            // PONG
            } else if(midTopic.equalsIgnoreCase("pong")) {
                // Ignore it.

            // RESET
            } else if(midTopic.equalsIgnoreCase("reset")) {
                // Respond if reset is addressed to us
                if(rightTopic.equalsIgnoreCase(_controllerName)) {
                    log("Reset addressed to us: "+_controllerName);
                    System.reset();
                }

            // MEMORY
            } else if(midTopic.equalsIgnoreCase("memory")) {
                // Respond if memory is addressed to us
                if(rightTopic.equalsIgnoreCase(_controllerName)) {
                    log("Memory addressed to us: "+_controllerName);
                    log( String::format("Free memory = %d", System.freeMemory()));
                }

            // LOG
            } else if(midTopic.equalsIgnoreCase("log")) {
                // Ignore it.

            // UNKNOWN
            } else {
                log("MQTT topic unknown");
            }
        }

    // Note: currently subscribing to _controllerName, so this will never happen
    } else if(topic.startsWith("smartthings")) {
        // Bridge may need to do something with this.
        log("MQTT smartthings received. Nothing to do.");

    }
}

