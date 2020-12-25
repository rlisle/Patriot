/**
state

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.
*/

#include "state.h"

State::State(String name, int value)
{
    _next  = NULL;
    _value = value;
    _name  = name;
    _previous = -1;
}

bool State::hasChanged() {
    return _value != _previous;
}

int State::value() {
    return _value;
}

void State::setValue(int value) {
    _value = value;
}

void State::setPrevious() {
    _previous = _value;
}
