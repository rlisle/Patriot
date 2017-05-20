/* Patriot-DHT library by Ron Lisle
 */

#include "Patriot-DHT.h"

/**
 * Constructor.
 */
PatriotDHT::PatriotDHT()
{
  // be sure not to call anything that requires hardware be initialized here, put those in begin()
}

/**
 * Example method.
 */
void PatriotDHT::begin()
{
    // initialize hardware
    Serial.println("called begin");
}

/**
 * Example method.
 */
void PatriotDHT::process()
{
    // do something useful
    Serial.println("called process");
    doit();
}

/**
* Example private method
*/
void PatriotDHT::doit()
{
    Serial.println("called doit");
}
