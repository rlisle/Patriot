/******************************************************************
Activity plugin

Features:
- Allows Alexa to control a 'name' without hardware.
- This is how 'Activities' are implemented.
- By attaching behaviors, can be controlled by other states.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
 2020-11-23: Initial version based on PseudoDevice
******************************************************************/

#include "PatriotActivity.h"

/**
 * Constructor
 */
Activity::Activity(String name) : Device(name)
{
    // nothing to do
}

void Activity::setPercent(int percent) {
    log("Activity setPercent "+String(percent)+", ignoring", LogDebug);
//    _percent = percent;
};

void Activity::stateDidChange(States *states) {

    int ourStateValue = 0;
    
    //TODO:
    // See if other states changed, and if behaviors affect our state

    // See what states thinks is our current value
    State* ourState = states->getStateWithName(_name);
    if (ourState != NULL) {
        
        ourStateValue = ourState->_value;
        // Is it the same as our setting?
        if (ourStateValue != _percent) {
            log("Activity: our state changed: "+String(ourStateValue)+". Setting _percent", LogDebug);
            _percent = ourStateValue;
        }
    }

    // If any other states changed, then see if it affects us
    int newLevel = _behaviors.stateDidChange(states);
    log("Activity " + _name + " stateDidChange behaviors setting newLevel "+String(newLevel), LogDebug);
    if(newLevel != _percent) {
        log("Activity " + _name + " stateDidChange publishing newLevel "+String(newLevel), LogDebug);
        _percent = newLevel;
        String topic = "patriot/" + _name;
        String message = String(_percent);
        publish(topic,message);
    }
}


