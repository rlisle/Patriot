/******************************************************************
Device abstract class

This virtual class provides a common header for all devices
such as Lights, Switches, Fans, etc.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2018-01-17: Add function for device type
2017-10-27: V2.0.0
2017-05-20: Provide default implementations for everything,
            so this class is not abstract anymore.
2017-05-19: Remove dependencies to allow use in plugin.
2017-05-15: Make devices generic
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle lib
2016-09-11: Initial version
******************************************************************/
#pragma once

#include <IoT.h>
#include "behaviors.h"

enum class DeviceType {
    Unknown,
    Fan,
    Light,
    Motor,
    NCD8Relay,
    Presence,
    Relay,
    Switch,
    TempHumidity,
    Ultrasonic,
    NCD8Light
};

class Device {
 protected:
    String     _name;
    DeviceType _type;
    int        _percent;
    int        _brightness;
    Behaviors  _behaviors;

 public:
    // Pointer to methods in IoT. These are set in IoT->addDevice()
    void (*log)(String message);
    void (*publish)(String topic, String message);

    // Note: refer to http://www.learncpp.com/cpp-tutorial/114-constructors-and-initialization-of-derived-classes/
    //       for an explanation of how derived constructor member initialization works.
    Device(String name = "", DeviceType type = DeviceType::Unknown) : _name(name), _type(type) { 
        _behaviors = Behaviors();
    }

    void addBehavior(Behavior *behavior) { 
        _behaviors.addBehavior(behavior); 
        //addToListOfSupportedStates(behavior->stateName);
    };

    void stateDidChange(States *states) {
        int newLevel = _behaviors.stateDidChange(states);
        setPercent(newLevel);
    }

    virtual String name() { return _name; };
    virtual DeviceType type() { return _type; };

    // This method can either read the device directly, or use a value
    // set in the loop() if continuous or asynchronous polling is used.
    virtual int getPercent() { return _percent; }

    // You will need to override this if you are creating an output device
    // This is the method that should control it.
    virtual void setPercent(int percent) { _percent = percent; };

    // Brightness is used to set the percent value when switch is called.
    virtual int getBrightness() { return _brightness; }
    virtual void setBrightness(int percent) { _brightness = percent; };

    // Turn a device on or off, using brightness for the on value
    virtual int getSwitch() { return _percent > 0; }
    virtual void setSwitch(int percent) { _percent = _brightness; };

    // These are just convenience methods
    virtual bool isOn() { return _percent > 0; };
    virtual bool isOff() { return _percent == 0; };

    virtual void setOn() { setPercent(100); };
    virtual void setOff() { setPercent(0); };

    // Perform things continuously, such as fading or slewing
    virtual void loop() {};
};
