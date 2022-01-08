/******************************************************************
PatriotChecklist plugin

Features:
- Read and set checklist state.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

******************************************************************/
#pragma once

#include "Particle.h"
#include <device.h>

class Checklist : public Device
{
private:
    long       _lastPollTime;

    bool      isTimeToCheckState();
    bool      didStateChange();
    void      notify();
    
public:
    Checklist(String name);

    void begin();
    void loop();
    
    void setValue(int percent);
};
