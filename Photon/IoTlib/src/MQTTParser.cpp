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

MQTTParser::MQTTParser(String controllerName, String publishName, Devices *devices, Behaviors *behaviors)
{
    _controllerName = controllerName;
    _publishName = publishName;
    _devices = devices;
    _behaviors = behaviors;
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
            // If it wasn't a device name, it must be an activity.
            int value = state.toInt();
            _behaviors->performActivity(name, value);

        } else {
            int firstSlash = topic.indexOf('/');
            int lastSlash = topic.lastIndexOf('/');
            if(firstSlash == -1 || lastSlash == -1 || firstSlash == lastSlash) {
                log("Message does not contain 2 slashes, so ignoring");
                return;
            }
            String midTopic = topic.substring(firstSlash+1,lastSlash);
            String rightTopic = topic.substring(lastSlash+1);

            // ACTIVITY
            if(midTopic.equalsIgnoreCase("activity")) {
                log("Setting activity " + rightTopic + " to " + message);
                int value = message.toInt();
                _behaviors->performActivity(rightTopic, value);

            // BRIGHTNESS
            } else if(midTopic.equalsIgnoreCase("brightness")) {
                Device* device = _devices->getDeviceWithName(rightTopic);
                if(device)
                {
                    log("Brightness " + rightTopic + " found, setting to " + message);
                    int percent = message.toInt();
                    device->setBrightness(percent);
                }

              // DEVICE (LEGACY)
              } else if(midTopic.equalsIgnoreCase("device")) {
                  Device* device = _devices->getDeviceWithName(rightTopic);
                  if(device)
                  {
                      log("Device " + rightTopic + " found, setting to " + message);
                      // Check for on/off and call set?
                      int percent = message.toInt();
                      device->setPercent(percent);
                  }

              // SWITCH
            } else if(midTopic.equalsIgnoreCase("switch")) {
                  Device* device = _devices->getDeviceWithName(rightTopic);
                  if(device)
                  {
                      log("Switch " + rightTopic + " found, setting to " + message);
                      // check for on/off
                      int percent = parseValue(message);
                      device->setSwitch(percent);
                  }

            // PING
            } else if(midTopic.equalsIgnoreCase("ping")) {
                // Respond if ping is addressed to us
                if(rightTopic.equalsIgnoreCase(_controllerName)) {
                    log("Ping addressed to us");
                    mqtt->publish(_publishName + "/pong/" + _controllerName, message);
                }

            // PONG
            } else if(midTopic.equalsIgnoreCase("pong")) {
                // Ignore it.

            // RESET
            } else if(midTopic.equalsIgnoreCase("reset")) {
                // Respond if reset is addressed to us
                if(rightTopic.equalsIgnoreCase(_controllerName)) {
                    log("Reset addressed to us");
                    System.reset();
                }

            // MEMORY
            } else if(midTopic.equalsIgnoreCase("memory")) {
                // Respond if memory is addressed to us
                if(rightTopic.equalsIgnoreCase(_controllerName)) {
                    log("Memory addressed to us");
                    log( String::format("Free memory = %d", System.freeMemory()));
                }

            } else if(midTopic.equalsIgnoreCase("log")) {
                // Ignore it.

            // UNKNOWN
            } else {
                log("Ttopic unknown");
            }
        }

    // Note: currently subscribing to _controllerName, so this will never happen
    } else if(topic.startsWith("smartthings")) {
        // Bridge may need to do something with this.
        log("Smartthings received. Nothing to do.");
    }
}

int MQTTParser::parseValue(String message)
{
    int value = 0;
    if(message.equalsIgnoreCase("on")) {
        return 100;
    } else if(message.equalsIgnoreCase("off")) {
        return 0;
    }
    return message.toInt();
}

void MQTTParser::log(String message)
{
    IoT::log("MQTTParser: " + message);
}
