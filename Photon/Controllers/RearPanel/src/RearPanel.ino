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
#include <PatriotSwitch.h>
#include <PatriotNCD8Light.h>

#define ADDRESS 1   // PWM board address A0 jumper set

String mqttServer = "192.168.10.184";

IoT *iot;

// To use persistent storage, insert "retained" before NCD8Relay
NCD8Light ceiling(ADDRESS, 0, "OfficeCeiling", 2);
NCD8Light loft(ADDRESS, 1, "Loft");
NCD8Light piano(ADDRESS, 2, "Piano", 2);
NCD8Light rampPorch(ADDRESS, 3, "RampPorch");
NCD8Light rampAwning(ADDRESS, 4, "RampAwning");
NCD8Light rearPorch(ADDRESS, 5, "RearPorch");
NCD8Light rearAwning(ADDRESS, 6, "RearAwning");


void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("RearPanel");
    iot->begin();
    iot->connectMQTT(mqttServer, "PatriotRearPanel1", true);   // MQTT bridge enabled

    // Behaviors/Activities
    Behavior *wakeup = new Behavior(10);
    wakeup->addCondition(new Condition("goodmorning", '>', 0));
    ceiling.addBehavior(wakeup);

    
    iot->addDevice(&ceiling);
    iot->addDevice(&loft);
    iot->addDevice(&piano);
    iot->addDevice(&rampPorch);
    iot->addDevice(&rampAwning);
    iot->addDevice(&rearPorch);
    iot->addDevice(&rearAwning);
    
}

void loop() {
    iot->loop();
}
