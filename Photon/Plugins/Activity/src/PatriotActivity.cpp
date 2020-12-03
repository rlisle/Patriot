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
Activity::Activity(String name) : Device(name, Activity)
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
    
    //TODO: execute behaviors.
    // In this case behaviors describe how to affect other activities
    // There should be only 1 behavior for an activity.
    int numBehaviors = _behaviors->count();
    for (x=0; x<numBehaviors; x++) {
        Behavior *behavior = _behaviors->getBehavior(x);
        
    }
    for (Behavior *ptr = _behaviors; ptr != NULL; ptr = ptr->_next)
    {
        //Currently just picking highest level.
        int newLevel = ptr->evaluateStates(states);
        level = max(level,newLevel);
    }

    
    
};

/**
 * stateDidChange
 * Override to prevent analyzing behaviors.
 * We will use the value of our base bahavior to set our percent.
 * But all other behaviors actually generate MQTT message to other activities.
 */
void Activity::stateDidChange(States *states) {

    // Did our state change? If not, all done here.
    State* ourState = states->getStateWithName(_name);
    if (ourState == NULL) return;
    if (ourState->_value == _percent) return;
    
    //TODO: detect and stop infinite looping

    // Update our level, and perform behaviors if any
    setPercent(ourState->_value);
}


