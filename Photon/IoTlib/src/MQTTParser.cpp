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

MQTTParser::MQTTParser(String controllerName, String publishName, Devices *devices)
{
    _controllerName = controllerName;
    _publishName = publishName;
    _devices = devices;
    _states = States();
}

void MQTTParser::parseMessage(String topic, String message, MQTT *mqtt)
{
    uint publishNameLength = _publishName.length();

    log("received: " + topic + ", " + message);

    if(topic.startsWith(_publishName)) {
        // LEGACY
        if(topic.length() == publishNameLength) {
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
            _devices->stateDidChange(name,_states);

        } else {
            int firstSlash = topic.indexOf('/');
            int lastSlash = topic.lastIndexOf('/');
            if(firstSlash == -1 || lastSlash == -1 || firstSlash == lastSlash) {
                log("Message does not contain 2 slashes, so ignoring");
                return;
            }
            String midTopic = topic.substring(firstSlash+1,lastSlash).toLowerCase();
            String rightTopic = topic.substring(lastSlash+1).toLowerCase();

            // STATE
            if(midTopic.equals("state")) {
                log("Setting state " + rightTopic + " to " + message);
                String name = rightTopic;
                int value = message.toInt();
                _states->addState(name,value);
                _devices->stateDidChange(name,states);

            // BRIGHTNESS
            } else if(midTopic.equals("brightness")) {
                Device* device = _devices->getDeviceWithName(rightTopic);
                if(device)
                {
                    log("Brightness " + rightTopic + " found, setting to " + message);
                    int percent = message.toInt();
                    device->setBrightness(percent);
                }

              // DEVICE (LEGACY)
              } else if(midTopic.equals("device")) {
                  Device* device = _devices->getDeviceWithName(rightTopic);
                  if(device)
                  {
                      log("Device " + rightTopic + " found, setting to " + message);
                      // Check for on/off and call set?
                      int percent = message.toInt();
                      device->setPercent(percent);
                  }

              // SWITCH
            } else if(midTopic.equals("switch")) {
                  Device* device = _devices->getDeviceWithName(rightTopic);
                  if(device)
                  {
                      log("Switch " + rightTopic + " found, setting to " + message);
                      // check for on/off
                      int percent = parseValue(message);
                      device->setSwitch(percent);
                  }

            // PING
            } else if(midTopic.equals("ping")) {
                // Respond if ping is addressed to us
                if(rightTopic.equalsIgnoreCase(_controllerName)) {
                    log("Ping addressed to us");
                    mqtt->publish(_publishName + "/pong/" + _controllerName, message);
                }

            // PONG
            } else if(midTopic.equals("pong")) {
                // Ignore it.

            // RESET
            } else if(midTopic.equals("reset")) {
                // Respond if reset is addressed to us
                if(rightTopic.equalsIgnoreCase(_controllerName)) {
                    log("Reset addressed to us");
                    System.reset();
                }

            // MEMORY
            } else if(midTopic.equals("memory")) {
                // Respond if memory is addressed to us
                if(rightTopic.equalsIgnoreCase(_controllerName)) {
                    log("Memory addressed to us");
                    log( String::format("Free memory = %d", System.freeMemory()));
                }

            } else if(midTopic.equals("log")) {
                // Ignore it.

            // UNKNOWN
            } else {
                log("Topic unknown");
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
