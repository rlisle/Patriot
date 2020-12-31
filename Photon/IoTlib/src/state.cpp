/**
state

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.
*/

#include "state.h"
#include "constants.h"  // Needed for particle.io states variable

/**
 * globalStatesVariable
 * Lists all the currently active states names in CSV format.
 */
String globalStatesVariable;


State::State(String name, int value)
{
    _next  = NULL;
    _value = value;
    _name  = name;
    _previous = -1;
}

int State::value() {
    return _value;
}

void State::setValue(int value) {
    _value = value;
}

bool State::hasChanged() {
    return _value != _previous;
}


// Static State collection methods

// States are added only once
State *State::addState(String name, int value) {
    // Update existing state if it exists
    State *state = getStateWithName(name);
    if (state == NULL) {
        Log.info("States addState adding " + name + " = " + String(value));
        state = new State(name,value);
        if(_states == NULL) {
            _states = state;
            expose();
        } else {
            State* ptr = _states;
            while(ptr->_next != NULL) ptr = ptr->_next;
            ptr->_next = state;
        }

    // If state already exists, then just update it
    } else {
        Log.info("States addState updating " + name + " = " + String(value) + ", was " + String(state->_value));
        state->_value = value;
    }
    Log.info("addState state was added. Count = " + String(count()));
    buildStatesVariable();
    return state;
}

State *State::getStateWithName(String name) {
    State *ptr = _states;
    while(ptr != NULL) {
        if (ptr->_name.equalsIgnoreCase(name)) {
            return ptr;
        }
        ptr = ptr->_next;
    }
    Log.info("getStateWithName " + name + " not found");
    return NULL;
}

int State::count() {
    int i = 0;
    for(State* ptr = _states; ptr != NULL; ptr = ptr->_next) i++;
    return i;
}

void State::syncPrevious() {
    for(State *ptr = _states; ptr != NULL; ptr = ptr->_next) {
        ptr->_previous = ptr->_value;
    }
}

// Particle.io States variable

void State::expose() {
    Log.info("Exposing States");
    globalStatesVariable = "";
    if (!Particle.variable(kStatesVariableName, globalStatesVariable)) {
        Log.error("Unable to expose " + String(kStatesVariableName) + " variable");
    }
}

void State::buildStatesVariable() {
    String newVariable = "";
    State *ptr = _states;
    while (ptr != NULL) {
        newVariable += ptr->_name;
        newVariable += ":";
        newVariable += String(ptr->_value);
        if (ptr->_next != NULL) {
            newVariable += ",";
        }
        ptr = ptr->_next;
    }
    if (newVariable.length() < kMaxVariableStringLength) {
        Log.info("Updating States variable");
        if (newVariable != globalStatesVariable) {
            globalStatesVariable = newVariable;
        }
    } else {
        Log.error("States variable is too long. Need to extend to a 2nd variable");
    }
}
