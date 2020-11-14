/******************************************************************
Behavior

This class represents a behavior, which is a response to a state
such as "tv" or "night". Multiple states can be specified, but
currently absent states (eg. this but not that) are not supported.
These may be easily added in the future.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2020-11-14: Rename activities to states
2017-10-22: Convert to scenes-like behavior
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle lib
2016-07-24: Initial version
******************************************************************/
#include "behavior.h"

Behavior::Behavior(Device *device, String stateName, char comparison, int value, int level)
{
    this->device = device;
    this->stateName = stateName;
    _comparison = comparison;     // '<', '=', '>', '!'
    _value = value;
    this->level = level;
}

bool Behavior::matchesCondition(String name, int value)
{
    if (name.equalsIgnoreCase(stateName))
    {
        switch (_comparison)
        {
            case '<':
                if (value < _value)
                {
                    return true;
                }
                break;
            case '=':
                if (value == _value)
                {
                    return true;
                }
                break;
            case '>':
                if (value > _value)
                {
                    return true;
                }
                break;
            case '!':
                if (value != _value)
                {
                    return true;
                }
                break;
            default:
                break;
        }
    }
    return false;
}

void Behavior::performState(String name, int value)
{
    if (matchesCondition(name, value))
    {
        //IoT* iot = IoT::getInstance();
        //iot->log("Behavior "+String(stateName)+" setting level "+String(level));
        device->setPercent(level);
    }
}
