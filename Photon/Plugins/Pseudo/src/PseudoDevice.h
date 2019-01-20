/******************************************************************
Patriot PseudoDevice plugin

Features:
- Allows Alexa to control a 'name' without hardware.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2019-01-20: Initial version
******************************************************************/
#pragma once

// This will load the definition for common Particle variable types
#include "device.h"

class PseudoDevice : public Device
{
public:
    /**
     * Constructor
     */
    PseudoDevice(String name);
    String name();
    int getPercent();
    bool isOn();
    void loop();

private:
    String     _name;
    bool       _isOn;
};
