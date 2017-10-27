/******************************************************************
Device abstract class

This virtual class provides a common header for all devices
such as Lights, Switches, Fans, etc.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
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

class Device {
 protected:
    String  _name;
    int     _percent;

 public:
    // Note: refer to http://www.learncpp.com/cpp-tutorial/114-constructors-and-initialization-of-derived-classes/
    //       for an explanation of how derived constructor member initialization works.
    Device(String name) : _name(name) { };

    virtual String name() { return _name; };

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

    // Each device may use different commands (eg. bright vs fast vs cold)
    // These need to be converted to a level by each device
    virtual int convertCommandToPercent(String command) { return 0; };

    // Perform things continuously, such as fading or slewing
    virtual void loop() {};
};
