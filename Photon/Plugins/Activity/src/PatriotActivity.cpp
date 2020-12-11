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

void Activity::setPercent(int percent) {
    Log.info("Activity " + _name + " setPercent "+String(percent)+", was "+String(_percent));
    _percent = percent;     // This is how we detect when it changes

    // Behaviors are only executed when an activity is turned on.
    if (percent == 0) return;

    int numActuators = _actuators.count();
    Log.info("Activity numActuators = "+String(numActuators));
    for(int x=0; x < numActuators; x++) {
        Actuator *actuator = _actuators.getActuatorAt(x);
        Log.info("Activity actuating "+actuator->_name);
        publish("patriot/"+actuator->_name, String(actuator->_value));
    }
};
