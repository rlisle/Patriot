/**
This is the primary class for the Patriot IoT library.

It aggregates all the other classes, and provides
a common API for adding and configuring devices.

This class coordinates realtime events.
It subscribes to Particle.io notifications, and distributes them to devices.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.
*/
#pragma once

#include "Particle.h"
#include "constants.h"
#include "device.h"
#include "MQTTManager.h"

// PartOfDay
#define SUNRISE 0
#define MORNING 1
#define NOON 2
#define AFTERNOON 3
#define SUNSET 4
#define DUSK 5
#define NIGHT 6
#define DAWN 7

// Sleeping
#define AWAKE 0
#define RETIRING 1
#define ASLEEP 2

/**
 * Main IoT object.
 * It provides an API for creating and communicating with all others.
 */
class IoT {

    friend MQTTManager;

public:

    /**
     * begin(byte * brokerIP, String connectID)
     * Call begin to initialize the object.
     * This is done here instead of the constructor to improve debugging.
     **/
    static void begin(String brokerIP, String controllerName);

    static void mqttPublish(String topic, String message);

    /**
     * Loop needs to be called periodically
     */
    static void loop();

    /**
    Values used by loop programming
     */
    static int  getChangedValue(String name);   // -1 if not changed
    
    static int setDeviceValue(String name, int value);
    
    static int handleLightSwitch(String name);        // Requires matching Light & LightSwitch type names
    
    static int publishValue(String name, int value);  // Sends MQTT message
    
private:
    static String _controllerName;
    static MQTTManager* _mqttManager;

    static void subscribeHandler(const char *eventName, const char *rawData);
    static void periodicReset();
    static void mqttHandler(char* topic, byte* payload, unsigned int length);
};
