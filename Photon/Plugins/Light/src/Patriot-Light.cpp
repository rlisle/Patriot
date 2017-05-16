/* Patriot-Light library by Ron Lisle
 */

#include "Patriot-Light.h"

/**
 * Constructor.
 */
PatriotLight::PatriotLight()
{
  // be sure not to call anything that requires hardware be initialized here, put those in begin()
}

/**
 * Example method.
 */
void PatriotLight::begin()
{
    // initialize hardware
    Serial.println("called begin");
}

/**
 * Example method.
 */
void PatriotLight::process()
{
    // do something useful
    Serial.println("called process");
    doit();
}

/**
* Example private method
*/
void PatriotLight::doit()
{
    Serial.println("called doit");
}
