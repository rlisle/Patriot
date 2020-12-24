/******************************************************************
Activity plugin

Features:
- Allows Alexa to control a 'name' without hardware.
- This is how 'Activities' are implemented.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

******************************************************************/

#include "PatriotActivity.h"

/**
 * Constructor
 */
Activity::Activity(String name) : Device(name, DeviceType::Activity)
{
    // nothing to do
}

void Activity::setPercent(int percent) {
    Log.info("Activity " + _name + " setPercent "+String(percent)+", was "+String(_percent));
    _percent = percent;     // This is how we detect when it changes
};
