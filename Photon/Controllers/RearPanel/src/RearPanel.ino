/*
 * RearPanel Controller
 * Description: This sketch controls all the switches in the Cyclone 4005 rear control panel.
 * Author: Ron Lisle
 * Date: 11/04/17
 *
 * To update Photon:
 *   1. Edit this code
 *   2. "particle flash RearPanel"
 *
 * Hardware
 * 1. NCD Photon Screw Terminal board
 *    6 switch connections:
 *      A0 Ceiling
 *      A1 Loft
 *      A2 Ramp Porch Floods
 *      A3 Ramp Awning LEDs
 *      A4 Rear Porch Flood
 *      A5 Rear Awning LEDs
 * 2. NCD 8 PWM OC 8W I2C Dimmer board
 *      A0 Ceiling
 *      A1 Loft
 *      A2 Ramp Porch Floods
 *      A3 Ramp Awning LEDs
 *      A4 Rear Porch Flood
 *      A5 Rear Awning LEDs
 *      A6 Piano Spot

 * Other
   - built-in blue LED     D7
 *
 * History
 * 11/22/20 Convert from relay board to Photon and Dimmer boards
 * 11/12/20 Remove 'retained' storage
 * 09/11/20 Add dimmer board support
 * 09/04/20 Change MQTT IP to 192.168.10.184
 * 08/29/20 Rename devices to remove spaces
 * 1/13/20 Add common activities
 * 5/4/19 Remove virtual devices. Using Home Assistant instead.
 * 03/30/19 Add virtual devices
 * 01/05/19 Use Backup SRAM to persist device states across resets.
 * 12/9/18  Removed all behaviors except 'everything'
 * 11/30/18 Restore behaviors.
 * 8/19/18  Remove all behaviors. Will perform those using scenes.
 * 6/20/18  Update to IoT 2.2.0 (mqtt) and MQTT IP to ...107
 * 3/16/18  Add MQTT
 * 12/02/17 Add retries (NCD8Relay v1.0.2)
 * 12/02/17 Installed 6 switches, changed relay assignments
 * 11/04/17 Initial files baseed on FrontPanel
 */
#include <IoT.h>
#include <PatriotLight.h>
#include <PatriotSwitch.h>
#include <PatriotNCD8Light.h>
#include <PatriotActivity.h>

#define ADDRESS 1   // PWM board address A0 jumper set

String mqttServer = "192.168.10.184";

IoT *iot;

// To use persistent storage, insert "retained" before NCD8Relay
NCD8Light ceiling(ADDRESS, 0, "OfficeCeiling", 1);
NCD8Light loft(ADDRESS, 1, "Loft");
NCD8Light piano(ADDRESS, 2, "Piano", 2);
NCD8Light rampPorch(ADDRESS, 3, "RampPorch", 2);
NCD8Light rampAwning(ADDRESS, 4, "RampAwning", 2);
NCD8Light rearPorch(ADDRESS, 5, "RearPorch", 2);
NCD8Light rearAwning(ADDRESS, 6, "RearAwning", 2);

Light blueLed(7, "blueLed", false, true);

Switch ceilingSwitch(A0, "OfficeCeilingSwitch");
Switch loftSwitch(A1, "LoftSwitch");
Switch rampPorchSwitch(A2, "RampPorchSwitch");
Switch rampAwningSwitch(A3, "RampAwningSwitch");
Switch rearPorchSwitch(A4, "RearPorchSwitch");
Switch rearAwningSwitch(A5, "RearAwningSwitch");

Activity watchingTV("watchingTV");
Activity goingToBed("goingToBed");
Activity sleeping("sleeping");

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("RearPanel");
    iot->begin();
    iot->connectMQTT(mqttServer, "PatriotRearPanel1", true);   // MQTT bridge enabled

    // BEHAVIORS
    ceiling.addBehavior(10, "goodmorning", '>', 0);
    blueLed.addBehavior(100, "loftswitch", '>', 0);
    
    goingToBed.addBehavior(0, "sleeping", '>', 0);
    
    // ADD ALL DEVICES
    iot->addDevice(&ceiling);
    iot->addDevice(&loft);
    iot->addDevice(&piano);
    iot->addDevice(&rampPorch);
    iot->addDevice(&rampAwning);
    iot->addDevice(&rearPorch);
    iot->addDevice(&rearAwning);
    iot->addDevice(&blueLed);
    
    iot->addDevice(&ceilingSwitch);
    iot->addDevice(&loftSwitch);
    iot->addDevice(&rampPorchSwitch);
    iot->addDevice(&rampAwningSwitch);
    iot->addDevice(&rearPorchSwitch);
    iot->addDevice(&rearAwningSwitch);
    
    iot->addDevice(&watchingTV);
    iot->addDevice(&goingToBed);
    iot->addDevice(&sleeping);
}

void loop() {
    iot->loop();
}
