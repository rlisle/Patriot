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

class Devices;

class Device {
    friend Devices;

 protected:
    String     _name;
    DeviceType _type;
    int        _percent;

    Device*    _next;       // Linked list

 public:
    // Pointer to methods in IoT. These are set in IoT->addDevice()
    void (*publishPtr)(String topic, String message);

    // Note: refer to http://www.learncpp.com/cpp-tutorial/114-constructors-and-initialization-of-derived-classes/
    //       for an explanation of how derived constructor member initialization works.
    Device(String name = "", DeviceType type = DeviceType::Unknown) 
            : _name(name), _type(type), _percent(0), _next(NULL)
    {
        // Nothing to do.
    }

    void publish(String topic, String message) {
        if(publishPtr != NULL) {
            publishPtr(topic, message);
        }
    }
    
    virtual String name() { return _name; };
    virtual DeviceType type() { return _type; };

    // This method can either read the device directly, or use a value
    // set in the loop() if continuous or asynchronous polling is used.
    virtual int getPercent() { return _percent; }

    // You will need to override this if you are creating an output device
    // This is the method that should control it.
    virtual void setPercent(int percent) { _percent = percent; };

    // These are just convenience methods
    virtual bool isOn() { return _percent > 0; };
    virtual bool isOff() { return _percent == 0; };

    virtual void setOn() { setPercent(100); };
    virtual void setOff() { setPercent(0); };

    // Perform things continuously, such as fading or slewing
    virtual void loop() {};
};
