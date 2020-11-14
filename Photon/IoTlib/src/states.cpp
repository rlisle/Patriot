/******************************************************************
States collection

This object manages a collection of State objects.

Multiple states can be active at the same time, so it is
important to combine their effects, in addition to stopping an
state in a manner that doesn't break states that continue
to be in effect.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2020-11-14: Rename activity to state
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle library
2016-12-04: Refactor to behaviors
2016-06-24: Initial version
******************************************************************/

#include "states.h"
#include "constants.h"

/**
 * globalStatesVariable
 * Lists all the currently active states names in CSV format.
 */
String globalStatesVariable;

States::States() {
    // Without this method, strange error is reported and build fails
    _numStates = 0;
    _isVariableExposed = false;
}

State *States::addState(String name, int value) {
    Serial.print("addState " + name + "=" + String(value));
    // Update existing state if it exists
    State *state = getStateWithName(name);
    if (state != NULL) {
        Serial.println(": updated");
        state->_value = value;

        // If not, create a new state
    } else {
        Serial.println(": added");
        if (_numStates < MAX_NUM_ACTIVITIES - 1) {
            state = new State(name, value);
            _states[_numStates++] = state;
        }
    }
    buildStateVariable();
    return state;
}

State *States::getStateByNum(int stateNum) {
    State *state = NULL;
    if (stateNum < _numStates) {
        state = _states[stateNum];
    }
    return state;
}

State *States::getStateWithName(String name) {
    for (int i = 0; i < _numStates; i++) {
        if (_states[i]->_name.equalsIgnoreCase(name)) {
            return _states[i];
        }
    }
    return NULL;
}

int States::count() {
    return _numStates;
}

bool States::expose() {
    _isVariableExposed = true;
    if (!Particle.variable(kStatesVariableName, globalStatesVariable)) {
        Serial.println("Unable to expose " + String(kStatesVariableName) + " variable");
        return false;
    }
    return true;
}

void States::buildStateVariable() {
    String newVariable = "";
    for (int i = 0; i < _numStates; i++) {
        Serial.print("state=");
        Serial.println(_states[i]->_name);
        newVariable += _states[i]->_name;
        newVariable += ":";
        Serial.print("value=");
        Serial.println(_states[i]->_value);
        newVariable += String(_states[i]->_value);
        if (i < _numStates - 1) {
            newVariable += ",";
        }
    }
    if (newVariable.length() < kMaxVariableStringLength) {
        if (newVariable != globalStatesVariable) {
            globalStatesVariable = newVariable;
        }
    } else {
        Serial.println("Variable is too long. Need to extend to a 2nd variable");
        Serial.println(newVariable);
        //WakeUp,WatchTV,Cook,Sleep
    }

}
