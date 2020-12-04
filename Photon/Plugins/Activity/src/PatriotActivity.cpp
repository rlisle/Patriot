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
    
    //TODO: execute behaviors.
    // In this case behaviors describe how to affect other activities
    int numBehaviors = _behaviors.count();
    if (numBehaviors > 1) log("Error: there should only be 1 Activity behavior, but found "+String(numBehaviors), LogError);
    
    Behavior *behavior = _behaviors.getBehaviorAt(0);
    Conditions *conditions = behavior->getConditions();
    int count = conditions->count();
    for(int x=0; x<count; x++) {
        Condition *condition = conditions->getCondition(x);
        String stateName = condition->_stateName;
        
        // and behaviors can only turn off other activities,
        // so infinite loops are not possible.
        // Later we'll use the operator and level for more complex behavior,
        // and we'll need to safeguard against loops then.
        //TODO: get kPublishName somehow instead of hardcoding "patriot"
        if (stateName != NULL) {
            publish("patriot/"+stateName, "0");
        } else {
            Log("Activity statename null");
        }
    }
    
};

/**
 * stateDidChange
 * Override to prevent analyzing behaviors.
 * We will use the value of our base bahavior to set our percent.
 * But all other behaviors actually generate MQTT message to other activities.
 */
void Activity::stateDidChange(States *states) {

    log("Activity stateDidChange", LogDebug);
    
    // Did our state change? If not, all done here.
    State* ourState = states->getStateWithName(_name);
    if (ourState == NULL) return;
    if (ourState->_value == _percent) return;
    
    //TODO: detect and stop infinite looping

    // Update our level, and perform behaviors if any
    setPercent(ourState->_value);
}


