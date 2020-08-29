/*
 * PatriotPi
 * Description: This sketch bridges between MQTT and Patriot.
 * Author: Ron Lisle
 * Date: 04/14/18
 *
 * This controller will bridge information between SmartThings on MQTT
 * to Particle.io via Wifi.
 * 
 * Initially it will expose SmartThings devices to Alexa by hardcoding
 * their device names. Later we'll identify devices by querying ST.
 * 
 * This controller will control devices based on activities
 * and input events such as door switches, time-of-day, temperature, etc.
 * 
 * This controller will also expose it's IP so other controllers can
 * determine the MQTT IP address to use.
 * 
 * To update Photon:
 *   1. Edit this code
 *   2. Validate with "particle compile pi"
 *   3. Upload with "particle flash ronspi"
 *
 * History
 * 7/20/18  Removed IoT. Not needed for RPi controller.
 * 4/14/18  Initial creation. Hardcoded DeskLamp device only.
 */

#include "MQTT.h"

//TODO: determine this dynamically, since it's running on this controller.
// Can use WiFi.localIP() to determine dynamically
// Then this variable can be a temporary/stack variable
byte brokerIP[] = {192,168,10,111}; // Pi IP = 0,101 on Tenda, 10,107 on TRENDNet

/**
 * Supported Activities and Devices variables
 * These variables are updated to contain a comma separated list of
 * the activity and device names supported by this controller.
 * This allows applications to automatically determine them.
 * Hardcoded SmartThings names for now.
 * Query ST for them would be preferred.
 */
String supportedActivitiesVariable = "";
String supportedDevicesVariable = "Desk Lamp";

String localIPVariable = "00.00.00.00";

MQTT *mqtt;

void setup() {
    // Do we want to setup Serial or use something else?
    // TODO:

    // Determine our local IP address
    localIPVariable = WiFi.localIP();

    // Parse IP 
    //TODO:

    // Connect to MQTT broker
    mqtt =  new MQTT(brokerIP, 1883, mqttHandler);
    mqtt->connect("PatriotIoT");   // This is NOT topic. Do we need something specific here?
    if (mqtt->isConnected()) {
        // Subscribe to all messages
        // Note: library doesn't support separate subscribes,
        //       so we'll filter them in the handler.
        if(!mqtt->subscribe("#")) {       // Topic name = everything.
            Serial.println("Unable to subscribe to MQTT");
        }
    }

    // Subscribe to events. There is a 1/second limit for events.
    Particle.subscribe("patriot", particleHandler, MY_DEVICES);

    // Register cloud variables. Up to 20 may be registered. Name length max 12.
    // There does not appear to be any time limit/throttle on variable reads.
    if(!Particle.variable("Supported", supportedActivitiesVariable))
    {
        Serial.println("Unable to expose Supported variable");
    }
    if(!Particle.variable("Devices", supportedDevicesVariable))
    {
        Serial.println("Unable to expose Devices variable");
    }
}

void loop() {
    // Keep the MQTT loop running
    if (mqtt != NULL && mqtt->isConnected()) {
        mqtt->loop();
    }
}

/**
 * Particle subscribe handler
 * Called by Particle.io when events are published.
 *
 * @param eventName
 * @param rawData
 */
void particleHandler(const char *eventName, const char *rawData)
{
    String data(rawData);
    String event(eventName);
    Serial.println("Subscribe handler event: " + event + ", data: " + data);
    int colonPosition = data.indexOf(':');
    String name = data.substring(0,colonPosition);
    String state = data.substring(colonPosition+1);

    // See if this is one of our devices. If so, translate to Patriot MQTT message.
    if(name == "desk lamp")
    {
        mqtt->publish("smartthings/DeskLamp/switch", state == "0" ? "off" : "on");
    }
}

/**
 * MQTT SmartThings subscribe handler
 * Called by MQTT when SmartThings events are published.
 *
 * The messages will be like: 'smartthings/DeskLamp/switch' 'on'
 *                        or: 'patriot', 'desklamp:100'
 *
 * @param topic
 * @param payload
 * @param length
 */
void mqttHandler(char *rawTopic, byte* rawPayload, unsigned int length) {
    // Convert passed data to Strings
    char payload[length + 1];
    memcpy(payload, rawPayload, length);
    payload[length] = 0;
    String data(payload);
    String topic(rawTopic);

    if(!strcmp(rawTopic,"patriot")) {
        int colonPosition = data.indexOf(':');
        String name = data.substring(0,colonPosition);
        String state = data.substring(colonPosition+1);
        if(name == "desklamp")
        {
            mqtt->publish("smartthings/DeskLamp/switch", state == "0" ? "off" : "on");
        }
        return;
    }

    if(!strncmp(rawTopic,"smartthings",11)) {
        int slash1Position = topic.indexOf('/');
        int slash2Position = topic.indexOf('/',slash1Position); //???
        String name = topic.substring(slash1Position,slash2Position-1);
        String type = topic.substring(slash2Position);
        // if(name == "DeskLamp") {
        //     String percent = (data == "on") ? "100" : "0";
        //     mqtt->publish("patriot", percent);
        // }
    }
}
