/*
 * OfficeCurtain
 * Description: This sketch controls all the motorized office curtain and LED strip lights.
 * Author: Ron Lisle
 * Date: 9/27/17
 *
 * Hardware
 *  1. Photon
 *  2. 5x5min Photon IoT v2 PCB with switching 5v regulator
 *  3. 2 Power MOSFET drivers:
 *    - Curtain open         (RX) MOSFET PWM dimmer to RJ11 pin 4 (green)
 *    - Curtain close        (TX) MOSFET PWM dimmer to RJ11 pin 2 (red)
 *                           Note: activating both = stop   pin 1 (black) GND
 *                                 Momentary pulse opens/closes, stop for partial
 *
 * History
 *  9/27/17 Initial creation using Patriot plugins
 */
#include <IoT.h>
#include <PatriotMotorized.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("OfficeCurtain");
    iot->setPublishName("patriot");
    iot->begin();

    Motorized *curtain = new Motorized(RX, TX, 5, "curtain");
    curtain->setPulseMode(100);
    iot->addDevice(curtain);

    // BEHAVIORS
    iot->addBehavior(new Behavior(curtain, "home", '>', 0, 100));       // Open curtains during day when home
    iot->addBehavior(new Behavior(curtain, "away", '>', 0, 0));         // Close curtains at when away
    iot->addBehavior(new Behavior(curtain, "nighttime", '>', 0, 0));    // Close curtains at night
    iot->addBehavior(new Behavior(curtain, "curtains", '>', 0, 100));   // Open curtains
}

void loop() {
    iot->loop();
}
