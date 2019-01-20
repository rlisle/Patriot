/******************************************************************
PatriotPseudo plugin

Features:
- Allows Alexa to control a 'name' without hardware.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2019-01-20: Initial version
******************************************************************/

#include "PatriotPseudo.h"

/**
 * Constructor
 */
Switch::Switch(String name)
{
    _name = name;
    _isOn = false;
}


/**
 * name()
 * @return string name of switch
 */
String Switch::name()
{
    return _name;
}

/**
 * isOn
 * @return bool true if switch is on
 */
bool Switch::isOn()
{
    return _isOn;
}


/**
 * getPercent()
 * @return
 */
int Switch::getPercent()
{
    return _isOn ? 100 : 0;
}


/**
 * loop()
 * This is called repeatedly to allow device to do its work.
 */
void Switch::loop()
{
    // Nothing to do
}
