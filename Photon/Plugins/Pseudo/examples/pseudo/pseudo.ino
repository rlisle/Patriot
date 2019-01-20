/*******************************************************
Patriot <YourDevice> Example

This example shows how to use the Patriot <YourDevice> plugin
It includes:

 * list the devices, pins, etc.
    - <name>                <pin>   Photon pin <pinNum>

http://www.github.com/<your repository>

Written by <Your Name>

<Your License info, such as:>
BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
<YYYY-MM-DD>: Initial creation
********************************************************/

#include <IoT.h>
#include <YourDevice.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("myPhoton");
    iot->begin();

    // Create your device
    YourDevice *yourDevice1 = new YourDevice(D4, "whatever");

    // Add it to IoT
    iot->addDevice(yourDevice1);

    // Add any behaviors if appropriate
    iot->addBehavior(new Behavior(yourDevice1, "YourCommand", '>', 0, 100));
    ...
}

void loop() {
    iot->loop();
}
