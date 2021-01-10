/**
 Period.cpp
 
 PatriotPartOfDay plugin

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

*/

#include "Period.h"

Period::Period(int hour, int minute) {
    _hour = hour;
    _minute = minute;
}

// hour and minute can be < 0 or too big and will be corrected
void Period::set(int hour, int minute) {
    _hour = hour;
    _minute = minute;
    if(_minute < 0) {
        _hour--;
        _minute += 60;
    }
    if(_minute >= 60) {
        _hour++;
        _minute -= 60;
    }
    if(_hour >= 24) {
        _hour -= 24;
    }
    if(_hour < 0) {
        hour += 24;
    }
}

bool Period::operator ==(const Period& period) {
    return period._hour == _hour && period._minute == _minute;
}

bool Period::operator >(const Period& period) {
    if(_hour > period._hour) return true;
    if(_hour < period._hour) return false;
    return _minute > period._minute;
}

bool Period::operator <(const Period& period) {
    if(_hour < period._hour) return true;
    if(_hour > period._hour) return false;
    return _minute < period._minute;
}

String Period::info() {
    return String(_hour) + ":" + String(_minute);
}
