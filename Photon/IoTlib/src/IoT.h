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
#include "device.h"
#include "MQTTManager.h"

// PartOfDay
#define UNINITIALIZED 0
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

public:

    /**
     * begin(byte * brokerIP, String connectID)
     * Call begin to initialize the object.
     * This is done here instead of the constructor to improve debugging.
     **/
    static void begin(String brokerIP, String controllerName, bool enableCloud, bool mqttLogging);
    
    static String controllerName() { return _mqttManager->controllerName(); };

    /**
     * Publish a message to MQTT
     * param: subtopic - MQTT topic prepended with "patriot/"
     * param message or value
     */
    static void publishMQTT(String subtopic, String message, bool retain = false);
    static void publishValue(String subtopic, int value, bool retain = false);

    /**
     * Loop needs to be called periodically
     */
    static void loop();
    
    
    static void setLatLong(float latitude, float longitude);
    static void setTimezone(int timezone);
    
    static void mqttHandler(char* topic, byte* payload, unsigned int length);
    
private:
    static String _controllerName;
    static MQTTManager* _mqttManager;
    static bool _cloudEnabled;
    static bool _mqttLogging;
    
    static void handleDaylightSavings();
    static void handleDSTMarch();
    static void handleDSTNovember();
    static void subscribeHandler(const char *eventName, const char *rawData);
};
