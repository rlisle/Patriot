/* Patriot-Switch library by Ron Lisle
 */

#include "Patriot-Switch.h"

/**
 * Constructor.
 */
PatriotSwitch::PatriotSwitch()
{
  // be sure not to call anything that requires hardware be initialized here, put those in begin()
}

/**
 * Example method.
 */
void PatriotSwitch::begin()
{
    // initialize hardware
    Serial.println("called begin");
}

/**
 * Example method.
 */
void PatriotSwitch::process()
{
    // do something useful
    Serial.println("called process");
    doit();
}

/**
* Example private method
*/
void PatriotSwitch::doit()
{
    Serial.println("called doit");
}
