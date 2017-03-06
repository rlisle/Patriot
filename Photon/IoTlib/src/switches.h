/******************************************************************
switches collection

Features:
- Manages a collection of Switch objects.

http://www.github.com/rlisle/ParticleIoT

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-05: Convert to v2 particle library
2016-07-30: Refactor to separate switch and switches classes
2016-06-21: Photon version
2016-02-3: Initial version
******************************************************************/
#pragma once

#include "application.h"
#include "switch.h"

#define kMaxNumSwitches 4     // Use multiple Switches objects if more needed

class Switches
{
public:
    int       addSwitch(int pinNum, String name);
    void      loop();

private:
    Switch    *_switchArray[4];
    int       _numSwitches;
};
