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

    friend void globalSubscribeHandler(const char *eventName, const char *rawData);
    friend void globalMQTTHandler(char *topic, byte* payload, unsigned int length);

public:

    /**
     * Singleton pattern
     * Use getInstance instead of trying to use the constructor (private)
     * @return
     */
    static IoT* getInstance();

    /**
     * Configuration methods
     * These are used to provide names and change defaults.
     * Only these methods may be called before begin()
     */
    void        setControllerName(String controllerName);

    /**
     * begin
     * Call begin to initialize the object.
     * This is done here instead of the constructor to improve debugging.
     */
    void        begin();

    /**
     * connectMQTT(byte * brokerIP, String connectID)
     * Connect to an MQTT broker with specified IP
     **/
    void        connectMQTT(String brokerIP, String connectID);

    void        mqttPublish(String topic, String message);

    /**
     * Loop needs to be called periodically
     */
    void loop();

    /**
    Values used by loop programming
     */
    bool handleLightSwitch(String name);        // Requires matching Light & LightSwitch type names
    
    int  publishValue(String name, int value);  // Sends MQTT message
    
    int  setDeviceValue(String name, int percent);   // returns 0 success else error code: -1 name not found

private:
    static IoT*  _instance;
    
    String       _controllerName;
    
    MQTTManager* _mqttManager;

    /**
     * Constructor
     * Private because this is a singleton
     */
    IoT();

    void subscribeHandler(const char *eventName, const char *rawData);
    void periodicReset();
    void mqttHandler(char* topic, byte* payload, unsigned int length);
};
