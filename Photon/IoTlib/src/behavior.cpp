/******************************************************************
Behavior

This class represents a behavior, which is a response to an activity
such as "tv" or "piano". Multiple activities can be specified, but
currently absent activities (eg. this but not that) are not supported.
These may be easily added in the future.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2017-10-22: Convert to scenes-like behavior
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle lib
2016-07-24: Initial version
******************************************************************/
#include "behavior.h"
//#include "IoT.h"


Behavior::Behavior(Device *device, String activityName, char comparison, int value, int level)
{
    this->device = device;
    this->activityName = activityName;
    _comparison = comparison;     // '<', '=', '>', '!'
    _value = value;
    this->level = level;
}

bool Behavior::matchesCondition(String name, int value)
{
    if (name.equalsIgnoreCase(activityName))
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

void Behavior::performActivity(String name, int value)
{
    if (matchesCondition(name, value))
    {
        //IoT* iot = IoT::getInstance();
        //iot->log("Behavior "+String(activityName)+" setting level "+String(level));
        device->setPercent(level);
    }
}
