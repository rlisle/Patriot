/*
 * Template Controller
 * Description: This sketch provides a template for creating controllers.
 * Author: Ron Lisle
 *
 * Hardware TODO: update this with actual H/W and pins used
 * 1. Photon
 * 2. 5x5min Photon IoT v2 PCB
 * 3. 4 x LED driver boards (left to right at top):
 *    - Piano spots     (A3)
 *    - Overhead lights (RX)
 *    - Booth spots     (TX)
 *    - Booth lamp      (WKP)
 *
 * ToDo:
 * 1. Booth proximity detector  ?
 * 2. Piano proximity detector  ?
 * 3. Motion detector           (A5)
 * 4. Light Sensor              (A4)
 *
 * History
 *  5/27/17 Update to plugins
 *  3/16/17 Convert to IoT library
 *  8/29/16 Create template based on sections from existing controllers.
 */
#include <IoT.h>
#include <PatriotLight.h>

IoT*     iot;

// setup() runs once, when the device is first turned on.
void setup() {
    // IoT will initialize the pins for all defined devices
    iot = IoT::getInstance();
    iot->setControllerName("<PutControllerNameHere>");
    iot->setPublishName("<PutEventNameHere>");
    iot->begin();

    // Add lights
    Light *light1 = new Light(TX, "<NameYourLedWhatever>");

    // Add it to IoT
    iot->addDevice(light1);

    // Add behaviors (all referenced devices must be already defined above)
    iot->addBehavior(new Behavior(light1, "<EventName>", '>', 0, 100));
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  iot->loop();
}
