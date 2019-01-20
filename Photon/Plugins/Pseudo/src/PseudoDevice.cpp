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
PseudoDevice::PseudoDevice(String name)
{
    _name = name;
    _isOn = false;
}


/**
 * name()
 * @return string name of switch
 */
String PseudoDevice::name()
{
    return _name;
}

/**
 * isOn
 * @return bool true if switch is on
 */
bool PseudoDevice::isOn()
{
    return _isOn;
}


/**
 * getPercent()
 * @return
 */
int PseudoDevice::getPercent()
{
    return _isOn ? 100 : 0;
}


/**
 * loop()
 * This is called repeatedly to allow device to do its work.
 */
void PseudoDevice::loop()
{
    // Nothing to do
}
