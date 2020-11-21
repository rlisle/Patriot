/******************************************************************
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

Changelog:
 2020-11-21: Delete publishName, implement new MQTT protocol
2020-11-14: Convert scene-like behaviors back to per-device summing
2020-11-14: Rename activities to states
2019-01-05: Removed watchdog timer due to OTA issues.
2019-01-01: Replace 2am reset with hardware watchdog timer.
2018-11-05: Refactor to MQTTmanager.
2018-10-15: Expose MQTT publish.
2018-03-27: Add MQTT reconnect
2018-01-17: Add functions for device state and type
2017-10-22: Convert to scene-like behavior
2017-05-15: Make devices generic
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle library
2016-11-24: Initial version
******************************************************************/
#pragma once

#include "Particle.h"
#include "constants.h"
#include "behavior.h"
#include "states.h"
#include "devices.h"
#include "devicenames.h"
#include "MQTTManager.h"
#include "MQTTParser.h"
#include "factory.h"

/**
 * Main IoT object.
 * It provides an API for creating and communicating with all others.
 */
class IoT {

    friend void globalSubscribeHandler(const char *eventName, const char *rawData);
    friend void globalMQTTHandler(char *topic, byte* payload, unsigned int length);
    friend void globalQOScallback(unsigned int data);

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
    void setControllerName(String controllerName);

    /**
     * begin
     * Call begin to initialize the object.
     * This is done here instead of the constructor to improve debugging.
     */
    void begin();

    /**
     * connectMQTT(byte * brokerIP, String connectID, bool isBridge = false)
     * Connect to an MQTT broker with specified IP
     **/
    void connectMQTT(String brokerIP, String connectID, bool isBridge = false);

    void mqttPublish(String topic, String message);
    
    /**
     * Loop needs to be called periodically
     */
    void loop();

    void addDevice(Device *device);

    /**
     * Helper methods
     */

    static void log(String msg);

private:
    static IoT* _instance;
    bool    _hasBegun;
    bool    _isBridge;
    String  _controllerName;
    String  _proximityEvent;
    system_tick_t _startTime;
    system_tick_t _currentTime;


    /**
     * Constructor
     * Private because this is a singleton
     */
    IoT();

    /**
     * Include other needed objects
     */
    Factory     *_factory;
    Devices     *_devices;    // "
    DeviceNames *_deviceNames;
    MQTTManager *_mqttManager;
    MQTTParser  *_mqttParser;

    void subscribeHandler(const char *eventName, const char *rawData);
    void periodicReset();

    void mqttHandler(char* topic, byte* payload, unsigned int length);
    void mqttQOSHandler(unsigned int data);
};
