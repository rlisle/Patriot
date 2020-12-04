/******************************************************************
Activity plugin

Features:
- Allows Alexa to control a 'name' without hardware.
- This is how 'Activities' are implemented.
- By attaching behaviors, can control other states/activities.

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

/**
 * This is called by our stateDidChange
 * This is how an Activity controls other activities
 * We may need to add code to prevent infinite loops
 */
void Activity::setPercent(int percent) {
    log("Activity setPercent "+String(percent)+", was "+String(_percent), LogDebug);
    _percent = percent;     // This is how we detect when it changes

    // Behaviors are only executed when an activity is turned on.
    if (percent == 0) return;

    int numActuators = _actuators.count();
    log("Activity numActuators = "+String(numActuators));
    for(int x=0; x < numActuators; x++) {
        Actuator *actuator = _actuators.getActuatorAt(x);
        log("Activity actuating "+actuator->_name);
        publish("patriot/"+actuator->_name, String(actuator->_value));
    }
};
