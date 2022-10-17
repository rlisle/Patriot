/******************************************************************
Device class

This class is the parent of all device classes
such as Lights, Switches, Fans, etc.

http://www.github.com/rlisle/Patriot
 
 Device types:
 * C curtain
 * D door switch
 * F fan
 * H humidity
 * L light
 * M motion detector
 * P presence/proximity detector
 * S switch
 * T temperature
 * X checklist state

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
    int        _value;      // Current value, typically percent 0-100
    String     _room;       // Room where located
    int        _previous;   // Value before getChanged called
    char       _type;       // Character designating device type: L, S, etc.
    int        _brightness; // Persisted value to use with next setOn (used by IoT, not device itself)

public:

    Device(String name, String room, char type = 'L');

    // begin() is called automatically when device is added.
    // Do any heavy lifting or publishing here and not in constructor.
    virtual void begin() {};
    
    // Perform hardware reset if possible
    virtual void reset() {};

    virtual String name() { return _name; };
    virtual String room() { return _room; };
    virtual String type() { return String(_type); };
    
    virtual int value() { return _value; };
    virtual void setValue(int value);
    
    virtual int brightness() { return _brightness; };
    virtual void setBrightness(int value) { _brightness = value; };

    virtual int  getChangedValue();
    
    virtual void setLatLong(float latitude, float longitude);

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
    static void    setAllLatLong(float latitude, float longitude);

    /**
     Particle.io variable "States"
     */
    static void buildDevicesVariable();
    static void buildStatusVariable();
    static void buildChecklistVariable();
    static void publishStates();
    static void expose();

};
