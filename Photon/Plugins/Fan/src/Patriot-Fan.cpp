/* Patriot-Fan library by Ron Lisle
 */

#include "Patriot-Fan.h"

/**
 * Constructor.
 */
PatriotFan::PatriotFan()
{
  // be sure not to call anything that requires hardware be initialized here, put those in begin()
}

/**
 * Example method.
 */
void PatriotFan::begin()
{
    // initialize hardware
    Serial.println("called begin");
}

/**
 * Example method.
 */
void PatriotFan::process()
{
    // do something useful
    Serial.println("called process");
    doit();
}

/**
* Example private method
*/
void PatriotFan::doit()
{
    Serial.println("called doit");
}
