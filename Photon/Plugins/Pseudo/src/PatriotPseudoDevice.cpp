/******************************************************************
PatriotPseudo plugin

Features:
- Allows Alexa to control a 'name' without hardware.
- This is how 'Activities' are implemented.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

******************************************************************/

#include "PatriotPseudoDevice.h"

/**
 * Constructor
 */
PseudoDevice::PseudoDevice(String name) : Device(name)
{
    // Nothing else to do
}
