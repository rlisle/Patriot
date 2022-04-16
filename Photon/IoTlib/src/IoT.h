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

// 0 means unitialized, initial state
// Setting to 0 may restore previous restoration

// PartOfDay
#define SUNRISE 1
#define MORNING 2
#define NOON 3
#define AFTERNOON 4
#define SUNSET 5
#define DUSK 6
#define NIGHT 7
#define DAWN 8

// Sleeping
#define AWAKE 1
#define RETIRING 2
#define ASLEEP 3

/**
 * Main IoT object.
 * It provides an API for creating and communicating with all others.
 * All methods and properties are static, meaning no initialization
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
    
    static void publishValue(String name, int value);  // Sends MQTT message
    
private:
    static String _controllerName;
    static MQTTManager* _mqttManager;
    
    static void subscribeHandler(const char *eventName, const char *rawData);
    static void mqttHandler(char* topic, byte* payload, unsigned int length);
    static void handleDaylightSavings();
    static void handleDSTMarch();
    static void handleDSTNovember();
};
