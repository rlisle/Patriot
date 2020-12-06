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
#include <PatriotLight.h>
#include <PatriotNCD8Relay.h>
#include <PatriotNCD8Light.h>
#include <PatriotActivity.h>

#define ADDRESS1 0x20   // Relay board
#define NUMRELAYS 8
#define ADDRESS2 1      // PWM board switches low switch on

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

NCD16Switch ceilingSwitch(A0, "CeilingSwitch");
NCD16Switch kitchenCeilingSwitch(A1, "KitchenCeilingSwitch");
NCD16Switch sinkSwitch(A2, "SinkSwitch");
Switch cabinetSwitch(A3, "CabinetSwitch");

Switch rightTrimSwitch(A4, "RightTrimSwitch");
Switch leftTrimSwitch(A5, "LeftTrimSwitch");

Switch dsFloodsSwitch(A6, "DSFloodsSwitch");
Switch odsFloodsSwitch(A7, "ODSFloodsSwitch");

Switch frontPorchSwitch(RX, "FrontPorchSwitch");
Switch frontAwningSwitch(TX, "FrontAwningSwitch");

// Activities allow Alexa to control them
// and can also turn off other activities.
// These will be used by other panels also, but don't need to be duplicated by them
//Activity waking("waking");                  // Turns off sleeping

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("FrontPanel");
    iot->begin();
    iot->connectMQTT(mqttServer, "patriotFrontPanel1");
    iot->setLogLevel(LogError);

    // Set other states
//    waking.setOtherState("sleeping", 0);        // Turn off sleeping when waking

    // BEHAVIORS

    // Waking
    ceiling.addBehavior(30, "CeilingSwitch", '>', 0);
    kitchenCeiling.addBehavior(30, "KitchenCeilingSwitch", '>', 0);
    kitchenSink.addBehavior(30, "SinkSwitch", '>', 0);
    kitchenCabinets.addBehavior(30, "CabinetSwitch", '>', 0);
    rightTrim.addBehavior(30, "RightTrimSwitch", '>', 0);
    leftTrim.addBehavior(30, "LeftTrimSwitch", '>', 0);
    dsFloods.addBehavior(30, "DSFloodsSwitch", '>', 0);
    odsFloods.addBehavior(30, "ODSFloodsSwitch", '>', 0);
    frontPorch.addBehavior(30, "FrontPorchSwitch", '>', 0);
    frontAwning.addBehavior(30, "FrontAwningSwitch", '>', 0);

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
    
    iot->addDevice(&ceilingSwitch);
    iot->addDevice(&kitchenCeilingSwitch);
    iot->addDevice(&sinkSwitch);
    iot->addDevice(&cabinetSwitch);
    iot->addDevice(&rightTrimSwitch);
    iot->addDevice(&leftTrimSwitch);
    iot->addDevice(&dsFloodsSwitch);
    iot->addDevice(&odsFloodsSwitch);
    iot->addDevice(&porchSwitch);
    iot->addDevice(&frontAwningSwitch);

}

void loop() {
    iot->loop();
}
