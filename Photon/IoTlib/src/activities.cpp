/******************************************************************
Activities collection

This object manages a collection of Activity objects.

Multiple activities can be active at the same time, so it is
important to combine their effects, in addition to stopping an
activity in a manner that doesn't break activities that continue
to be in effect.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle library
2016-12-04: Refactor to behaviors
2016-06-24: Initial version
******************************************************************/

#include "activities.h"
#include "constants.h"

/**
 * globalActivitiesVariable
 * Lists all the currently active activities names in CSV format.
 */
String globalActivitiesVariable;

Activities::Activities() {
    // Without this method, strange error is reported and build fails
    _numActivities = 0;
    _isVariableExposed = false;
}

Activity *Activities::addActivity(String name, int value) {
    Serial.println("addActivity " + name + "=" + String(value));
    // Update existing activity if it exists
    Activity *activity = getActivityWithName(name);
    if (activity != NULL) {
        Serial.println("   updating activity");
        activity->_value = value;

        // If not, create a new activity
    } else {
        Serial.println("   adding new activity");
        if (_numActivities < MAX_NUM_ACTIVITIES - 1) {
            activity = new Activity(name, value);
            _activities[_numActivities++] = activity;
        }
    }
    buildActivityVariable();
    return activity;
}

Activity *Activities::getActivityByNum(int activityNum) {
    Activity *activity = NULL;
    if (activityNum < _numActivities) {
        activity = _activities[activityNum];
    }
    return activity;
}

Activity *Activities::getActivityWithName(String name) {
    for (int i = 0; i < _numActivities; i++) {
        if (_activities[i]->_name.equalsIgnoreCase(name)) {
            return _activities[i];
        }
    }
    return NULL;
}

int Activities::count() {
    return _numActivities;
}

bool Activities::expose() {
    _isVariableExposed = true;
    if (!Particle.variable(kActivitiesVariableName, globalActivitiesVariable)) {
        Serial.println("Unable to expose " + String(kActivitiesVariableName) + " variable");
        return false;
    }
    return true;
}

void Activities::buildActivityVariable() {
    String newVariable = "";
    for (int i = 0; i < _numActivities; i++) {
        newVariable += _activities[i]->_name;
        newVariable += ":";
        newVariable += String(_activities[i]->_value);
        if (i < _numActivities - 1) {
            newVariable += ",";
        }
    }
    if (newVariable.length() < kMaxVariableStringLength) {
        if (newVariable != globalActivitiesVariable) {
            Serial.println("Activities=" + newVariable);
            globalActivitiesVariable = newVariable;
        }
    } else {
        Serial.println("Variable is too long. Need to extend to a 2nd variable");
    }

}
