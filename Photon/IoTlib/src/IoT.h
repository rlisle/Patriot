/**
This is the primary class for the Patriot IoT library.

It aggregates all the other classes, and provides
a common API for adding and configuring devices.

This class coordinates realtime events.
It subscribes to Particle.io notifications, and
        distributes them to devices and states.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.
*/
#pragma once

#include "Particle.h"
#include "constants.h"
#include "behavior.h"
#include "states.h"
#include "devices.h"
#include "MQTTManager.h"
#include "PLogLevel.h"

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
     * connectMQTT(byte * brokerIP, String connectID, bool isBridge = false)
     * Connect to an MQTT broker with specified IP
     **/
    void        connectMQTT(String brokerIP, String connectID, bool isBridge = false);

    void        mqttPublish(String topic, String message);
    void        addDevice(Device *device);
    void        setLogLevel(PLogLevel logLevel);
    void        log(String msg, PLogLevel logLevel = LogDebug);

    /**
     * Loop needs to be called periodically
     */
    void loop();


private:
    static IoT*  _instance;
    
    bool         _isBridge;
    String       _controllerName;
    PLogLevel    _logLevel;
    
    States*      _states;
    Devices*     _devices;
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
