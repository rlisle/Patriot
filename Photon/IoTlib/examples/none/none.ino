/******************************************************************
Example starting point sketch for Patriot library

This example contains no devices.
It is provided as a starting point for you own sketches.

In order to use any devices, you will need to include the plugin
library that supports each device. Refer to the examples provided
with each plugin.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-05-18: Add comment regarding plugins.
2017-03-24: Rename Patriot
2017-03-11: Initial creation
******************************************************************/

#include <IoT.h>
IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("myController");
    iot->setPublishName("myEvent");
    iot->begin();
}

void loop() {
    iot->loop();
}
