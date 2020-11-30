/**
  FrontPanel Controller
  Description: This sketch controls all the switches in the Cyclone 4005 front control panel.
  Author: Ron Lisle
  Date: 9/16/17
 
  To update Photon:
    1. Edit this code
    2. "particle flash FrontPanel"
 
  Hardware
  1. Particle.io Photon
  2. I2C cables between boards
  3. NCD.io NCD8Relay Photon Controller 0x20
    - Front Awning LEDs    (2)
    - Right Trim           (3)
    - Left Trim            (4)
    - Door Side Floods     (6)
    - Other Side Floods    (7)
    - Porch                (8)
    - ?
    - ?
  4. NCD.io NCD8Light I2C 8x Dimmer Controller
    - Ceiling
    - Kitchen Ceiling
    - Sink
    - Cabinets
    - ?
    - ?
    - ?
    - ?
 */
#include <IoT.h>
#include <PatriotNCD8Relay.h>
#include <PatriotNCD8Light.h>
#include <PatriotNCD16Switch.h>
#include <PatriotActivity.h>

#define ADDRESS1 0x20   // Relay board
#define NUMRELAYS 8
#define ADDRESS2 1      // PWM board switches low switch on
#define ADDRESS3 0x20   // 16x Switch No jumpers set - WILL THIS CONFLICT WITH RELAY BOARD?

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

NCD16Switch cabinetSwitch(ADDRESS3, 1, "CabinetSwitch");
NCD16Switch rightTrimSwitch(ADDRESS3, 2, "RightTrimSwitch");

NCD16Switch sinkSwitch(ADDRESS3, 3, "SinkSwitch");
NCD16Switch ceilingSwitch(ADDRESS3, 4, "CeilingSwitch");
NCD16Switch leftTrimSwitch(ADDRESS3, 5, "LeftTrimSwitch");

NCD16Switch kitchenCeilingSwitch(ADDRESS3, 6, "KitchenCeilingSwitch");
NCD16Switch dsFloodsSwitch(ADDRESS3, 7, "DSFloodsSwitch");
NCD16Switch odsFloodsSwitch(ADDRESS3, 8, "ODSFloodsSwitch");
NCD16Switch porchSwitch(ADDRESS3, 9, "PorchSwitch");

NCD16Switch frontAwningSwitch(ADDRESS3, 10, "FrontAwningSwitch");

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
    
    iot->addDevice(&cabinetSwitch);
    iot->addDevice(&rightTrimSwitch);
    iot->addDevice(&sinkSwitch);
    iot->addDevice(&ceilingSwitch);
    iot->addDevice(&leftTrimSwitch);
    iot->addDevice(&kitchenCeilingSwitch);
    iot->addDevice(&dsFloodsSwitch);
    iot->addDevice(&odsFloodsSwitch);
    iot->addDevice(&porchSwitch);
    iot->addDevice(&frontAwningSwitch);

}

void loop() {
    iot->loop();
}
