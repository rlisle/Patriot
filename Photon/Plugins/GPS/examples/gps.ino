/*******************************************************
Patriot PartOfDay Example

This example shows how to use the Patriot PartOfDay plugin
It instantiates the plugin which will then generate
MQTT messages/states for "partofday" with the values:
  * Dawn
  * Sunrise
  * Morning
  * Noon
  * Afternoon
  * Sunset
  * Dusk
  * Night

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

********************************************************/

#include <IoT.h>
#include <PatriotPartOfDay.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->begin();

    // Create DateTime device
    iot->addDevice(new PartOfDay());
}

void loop() {
    iot->loop();
}
