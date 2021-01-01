/******************************************************************
Device abstract class

This virtual class provides a common header for all devices
such as Lights, Switches, Fans, etc.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

******************************************************************/
#pragma once

#include "Particle.h"

enum class DeviceType {
    Unknown,
    Activity,
    Fan,
    Light,
    Motor,
    NCD8Light,
    NCD8Relay,
    NCD8Switch,
    NCD16Switch,
    PartOfDay,
    Presence,
    Relay,
    Switch,
    TempHumidity,
    Ultrasonic
};

class Device {
 protected:
    Device*    _next;       // Linked list
    String     _name;
    DeviceType _type;
    int        _percent;
    int        _previous;

public:
    // Pointer to MQTT publish method in IoT.
    void (*publishPtr)(String topic, String message);

    // Note: refer to http://www.learncpp.com/cpp-tutorial/114-constructors-and-initialization-of-derived-classes/
    //       for an explanation of how derived constructor member initialization works.
    Device(String name = "", DeviceType type = DeviceType::Unknown) 
            : _next(NULL), _name(name), _type(type), _percent(0)
    {
        // Do any setup work in begin()
    }

    // Can not be called in the constructor.
    void publish(String topic, String message) {
        if(publishPtr != NULL) {
            publishPtr(topic, message);
        }
    }
    
    // begin() is called automatically when device is added to IoT using addDevice() after the publishPtr has been set.
    // Do any heavy lifting or publishing here and not in constructor.
    virtual void begin() {};
    
    // Perform hardware reset if possible
    virtual void reset() {};

    virtual String name() { return _name; };
    virtual DeviceType type() { return _type; };
    
    // Allow similar code between States and Devices
    int value() { return _percent; };

    virtual int getPercent() { return _percent; }
    virtual void setPercent(int percent) { _percent = percent; };

    virtual bool hasChanged() { return _percent != _previous; };
    virtual void syncPrevious() { _previous = _percent; };
    
    // Perform things continuously, such as fading or slewing
    virtual void loop() {};
    
    //
    // Collection methods (previously in Devices)
    //
    static Device* _devices;
    
    static void resetAll();
    static void loopAll();
    static void syncAllPrevious();

    static void    add(Device *device);
    static Device* get(String name);
    static int     count();
    
    /**
     Particle.io variable "States"
     */
    static void buildDevicesVariable();
    static void expose();

};
