/******************************************************************
Device class

This class is the parent of all device classes
such as Lights, Switches, Fans, etc.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

******************************************************************/
#pragma once

#include "Particle.h"

class Device {
 protected:
    Device*    _next;       // Linked list
    String     _name;       // Public name (used by Alexa)
    int        _value;      // Typically percent 0-100
    int        _previous;   // Value at start of loop

public:
    // Pointer to MQTT publish method
    void (*publishPtr)(String topic, String message);

    Device(String name);

    // Doesn't work if called in the constructor.
    // because publishPtr is set afterwards, but before begin()
    void publish(String topic, String message);
    
    // begin() is called automatically when device is added.
    // This occurs after publishPtr is set.
    // Do any heavy lifting or publishing here and not in constructor.
    virtual void begin() {};
    
    // Perform hardware reset if possible
    virtual void reset() {};

    virtual String name() { return _name; };
    
    virtual int value() { return _value; };
    virtual void setValue(int value);

    virtual bool hasChanged() { return _value != _previous; };
    virtual void syncPrevious() { _previous = _value; };
    
    // Perform things continuously, such as fading or slewing
    virtual void loop() {};
    
    //
    // Static Collection Methods (previously in Devices)
    //
    static Device* _devices;
    
    static void resetAll();
    static void loopAll();
    static void syncAllPrevious();

    static void    add(Device *device);
    static Device* get(String name);
    static void    setValue(String name, int value);
    static int     count();
    
    /**
     Particle.io variable "States"
     */
    static void buildDevicesVariable();
    static void buildStatesVariable();
    static void expose();
    static void exposeStates();

};
