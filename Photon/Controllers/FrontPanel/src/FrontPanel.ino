/*
 * FrontPanel Controller
 * Description: This sketch controls all the switches in the Cyclone 4005 front control panel.
 * Author: Ron Lisle
 * Date: 9/16/17
 *
 * To update Photon:
 *   1. Edit this code
 *   2. "particle flash FrontPanel"
 *
 * Hardware
 * 1. Photon
 * 2. ControlEverything.com NCD8Relay Photon Controller
 * 3. ControlEverything.com NCD8Relay I2C 8x Dimmer Controller
 * 4. I2C cable between boards
 * Photon board 8x relay n/o connections 0x20
   - Front Awning LEDs    (2)
   - Right Trim           (3)
   - Left Trim            (4)
   - Door Side Floods     (6)
   - Other Side Floods    (7)
   - Porch                (8)
   - ?
   - ?
 * I2C 8x dimmer board connections 0x21
   - Ceiling
   - Kitchen Ceiling
   - Sink
   - Cabinets
   - ?
   - ? 
   - ?
   - ?
 * States
   - arriving
   - cleaning
   - cooking
   - evening / sunsetting
   - hosting
   - leaving
   - morning / sunrising
   - reading 
   - retiring (bedtime)
   - sleeping
   - waking
   - watching

 * History
 * 11/28/20 Update to v5
 * 11/09/20 Add dimmer board support
 * 09/04/20 Change MQTT IP to 192.168.10.184
 * 08/29/20 Remove spaces in device names
 * 01/05/19 v3.0.0 Save device state in Backup SRAM
 * 12/9/18  Remove behaviors except 'everything'
 * 11/30/18 Rename trim behavior to mood
 * 10/04/18 Add ping/pong support
 * 6/20/18  Update to IoT 2.2.0 (mqtt support) and MQTT IP to ...107
 * 3/30/18  Add MQTT support
 * 11/04/17 Rename TV lights to avoid conflict with Harmony TV commands.
 * 10/28/17 Update to v2
 * 10/17/17 Correct RV Lights to TV Lights.
 * 10/03/17 Convert to ConnectEverything relay boards
 *  9/16/17 Initial creation using Patriot plugin
 */
#include <IoT.h>
#include <PatriotNCD8Relay.h>
#include <PatriotNCD8Light.h>

#define ADDRESS1 0x20
#define ADDRESS2 1    // I2C PWM board switches low switch on
#define NUMRELAYS 8

String mqttServer = "192.168.10.184";

IoT *iot;

// To use persistent storage, insert "retained" before NCD8Relay
NCD8Relay frontAwning(ADDRESS1, NUMRELAYS, 1, "FrontAwning");
NCD8Relay rightTrim(ADDRESS1, NUMRELAYS, 2, "RightTrim");
NCD8Relay leftTrim(ADDRESS1, NUMRELAYS, 3, "LeftTrim");
NCD8Relay dsFloods(ADDRESS1, NUMRELAYS, 5, "DoorSide");
NCD8Relay osFloods(ADDRESS1, NUMRELAYS, 6, "OtherSide");
NCD8Relay frontPorch(ADDRESS1, NUMRELAYS, 7, "FrontPorch");

NCD8Light ceiling(ADDRESS2, 0, "Ceiling", 2);
NCD8Light kitchenCeiling(ADDRESS2, 1, "kitchenCeiling", 2);
NCD8Light kitchenSink(ADDRESS2, 2, "Sink", 2);
NCD8Light kitchenCabinets(ADDRESS2, 3, "Cabinets", 2);

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("FrontPanel");
    iot->begin();
    iot->connectMQTT(mqttServer, "patriotFrontPanel1");

    // BEHAVIORS

    // Waking
    //ceiling.addBehavior(100, "waking", '>', 0);

    // DEVICES

    iot->addDevice(&dsFloods);
    iot->addDevice(&frontAwning);
    iot->addDevice(&frontPorch);
    iot->addDevice(&leftTrim);
    iot->addDevice(&osFloods);
    iot->addDevice(&rightTrim);

    iot->addDevice(&ceiling);
    iot->addDevice(&kitchenCeiling);
    iot->addDevice(&kitchenSink);
    iot->addDevice(&kitchenCabinets);
}

void loop() {
    iot->loop();
}
