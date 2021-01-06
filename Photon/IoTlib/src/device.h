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

    Device(String name);

    // begin() is called automatically when device is added.
    // Do any heavy lifting or publishing here and not in constructor.
    virtual void begin() {};
    
    // Perform hardware reset if possible
    virtual void reset() {};

    virtual String name() { return _name; };
    
    virtual int value() { return _value; };
    virtual void setValue(int value);
    
    virtual int  getChangedValue();
    
    // Perform things continuously, such as fading or slewing
    virtual void loop() {};
    
    //
    // Static Collection Methods (previously in Devices)
    //
    static Device* _devices;
    
    static void resetAll();
    static void loopAll();

    static void    add(Device *device);
    static Device* get(String name);
    static int     value(String name);
    static int     setValue(String name, int value);
    static int     getChangedValue(String name);
    static int     count();
    
    /**
     Particle.io variable "States"
     */
    static void buildDevicesVariable();
    static String publishStates(String controllerName);
    static void expose();

};
