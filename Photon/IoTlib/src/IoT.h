/******************************************************************
This is the primary class for the IoT library.

It aggregates all the other classes, and provides
a common API for adding and configuring devices.

This class coordinates realtime events.
It subscribes to Particle.io notifications, and
        distributes them to devices and activities.

http://www.github.com/rlisle/ParticleIoT

Written by Ron Lisle, ron@lisles.net

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-05: Convert to v2 particle library
2016-11-24: Initial version
******************************************************************/
#pragma once

#include "Particle.h"
#include "constants.h"
#include "alive.h"
#include "activities.h"

/**
 * Main IoT object.
 * It provides an API for creating and communicating with all others.
 */
class IoT {

    friend void globalSubscribeHandler(const char *eventName, const char *rawData);

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
     * Loop needs to be called periodically
     */
    void loop(long millis);

    /**
     * Helper methods
     */

    void log(String msg);

private:
    static IoT* _instance;
    bool   _hasBegun;
    String _publishName;
    String _controllerName;

    /**
     * Constructor
     * Private because this is a singleton
     */
    IoT();

    /**
     * Include other needed objects
     */
    Alive       *_alive;
    Activities  *_activities;
//    Devices     *_devices;
//    DeviceNames *_deviceNames;
//    ControllerNames *_controllerNames;
//    Presence    *_presence;
//    Proximity   *_proximity;
//    Temperature *_temperature;
//    Switches    *_switches;       // lazy loaded

    void subscribeHandler(const char *eventName, const char *rawData);
};
