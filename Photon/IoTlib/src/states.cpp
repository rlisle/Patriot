/******************************************************************
States collection

This object manages a collection of State objects.

Multiple states can be active at the same time, so it is
important to combine their effects, in addition to stopping a
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
    _states = NULL;
    expose();
}

// States are added only once
State *States::addState(String name, int value) {
//    Serial.print("addState " + name + "=" + String(value));
    // Update existing state if it exists
    State *state = getStateWithName(name);
    if (state == NULL) {
//        Serial.println(": adding");
        State *state = new State(name,value);
        if(_states == NULL) {
            _states = state;
        } else {
            State* ptr = _states;
            while(ptr->_next != NULL) ptr = ptr->_next;
            ptr->_next = state;
        }
    } else {    // State already exists
//        Serial.println(": updated");
        state->_value = value;
    }

    buildStatesVariable();
    return state;
}

State *States::getStateWithName(String name) {
    State *ptr = _states;
    while(ptr != NULL) {
        if (ptr->_name.equalsIgnoreCase(name)) {
            return ptr;
        }
        ptr = ptr->_next;
    }
    return NULL;
}

int States::count() {
    int i = 0;
    for(State* ptr = _states; ptr != NULL; ptr = ptr->_next) i++;
    return i;
}

void States::expose() {
    if (!Particle.variable(kStatesVariableName, globalStatesVariable)) {
        Serial.println("Unable to expose " + String(kStatesVariableName) + " variable");
    }
}

void States::buildStatesVariable() {
    String newVariable = "";
    State *ptr = _states;
    while (ptr != NULL) {
        newVariable += ptr->_name;
        newVariable += ":";
        newVariable += String(ptr->_value);
        if (ptr->_next != NULL) {
            newVariable += ",";
        }
    }
    if (newVariable.length() < kMaxVariableStringLength) {
        if (newVariable != globalStatesVariable) {
            globalStatesVariable = newVariable;
        }
    } else {
        Serial.println("States variable is too long. Need to extend to a 2nd variable");
        Serial.println(newVariable);
    }
}
