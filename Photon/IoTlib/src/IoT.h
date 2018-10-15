/******************************************************************
This is the primary class for the Patriot IoT library.

It aggregates all the other classes, and provides
a common API for adding and configuring devices.

This class coordinates realtime events.
It subscribes to Particle.io notifications, and
        distributes them to devices and activities.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
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
#include "activities.h"
#include "behaviors.h"
#include "devices.h"
#include "devicenames.h"
#include "MQTT.h"

/**
 * Main IoT object.
 * It provides an API for creating and communicating with all others.
 */
class IoT {

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
    void setPublishName(String publishName);

    void setControllerName(String controllerName);

    /**
     * begin
     * Call begin to initialize the object.
     * This is done here instead of the constructor to improve debugging.
     */
    void begin();

    /**
     * connectMQTT(byte * brokerIP)
     * Connect to an MQTT broker with specified IP
     **/
    void connectMQTT(byte *brokerIP, String connectID, bool isBridge = false);
    void mqttPublish(String topic, String message);
    
    /**
     * Loop needs to be called periodically
     */
    void loop();

    void addDevice(Device *device);
    void addBehavior(Behavior *behavior);

    /**
     * Helper methods
     */

    static void log(String msg);

private:
    static IoT* _instance;
    bool    _hasBegun;
    bool    _isBridge;
    String  _publishName;
    String  _controllerName;
    String  _proximityEvent;
    String  _supportedActivities[kMaxNumberActivities];
    int     _numSupportedActivities;

    /**
     * Constructor
     * Private because this is a singleton
     */
    IoT();

    /**
     * Include other needed objects
     */
    Activities  *_activities;
    Behaviors   *_behaviors;
    Devices     *_devices;
    DeviceNames *_deviceNames;
    MQTT        *_mqtt;

    void subscribeHandler(const char *eventName, const char *rawData);
    void addToListOfSupportedActivities(String activity);
    void buildSupportedActivitiesVariable();
    void performActivities();   //TODO: To be deprecated

    void mqttHandler(char* topic, byte* payload, unsigned int length);

    int  programHandler(String command);
    int  valueHandler(String deviceName);
    int  typeHandler(String deviceName);
};
