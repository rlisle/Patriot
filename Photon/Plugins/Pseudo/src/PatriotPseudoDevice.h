/******************************************************************
Patriot PseudoDevice plugin

Features:
- Allows Alexa to control a 'name' without hardware.
- This is how 'Activities' are implemented.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2020-11-23: Update to v5 for use with Activities
2019-01-20: Initial version
******************************************************************/
#pragma once

#include "Particle.h"
#include "device.h"

class PseudoDevice : public Device
{
public:
    /**
     * Constructor
     */
    PseudoDevice(String name);

};
