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
 * 1. Photon
 * 2. ControlEverything.com NCD8Relay Photon Controller
 * 3. 2 x MOSFET LED drivers
 * Photon board relay n/o connections 0x20 (D0, D1)
   - Rear Awning          (1)
   - Rear Porch           (2)
   - Ramp Awning          (3)
   - Ramp Porch           (4)
   - Loft                 (5)
   - Office               (6)
   - Rear Awning Extend
   - Rear Awning Retract
   - Ramp Awning Extend
   - Ramp Awning Retract
 * MOSFET LED Driver Boards
   - Office Ceiling        (TODO: D3)
   - Loft                  (TODO: D4)
 * Other
   - built-in blue LED     D7
 *
 * Activities
   - arriving
   - cleaning
   - cooking
   - evening
   - everything
   - hosting
   - leaving
   - morning
   - reading 
   - retiring (bedtime)
   - sleeping
   - waking
   - watching
 *
 * History
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
#include <PatriotNCD8Relay.h>
//#include <PatriotNCD8Light.h>

#define DEV_PTR (Device *)&
#define ADDRESS1 0x20
#define ADDRESS2 1   // I2C PWM board switches low switch on
#define NUMRELAYS 8

String mqttServer = "192.168.1.10";

IoT *iot;

retained NCD8Relay loft(ADDRESS1, NUMRELAYS, 4, "Loft");
retained NCD8Relay office(ADDRESS1, NUMRELAYS, 5, "Office Ceiling");
retained NCD8Relay rampAwning(ADDRESS1, NUMRELAYS, 2, "Ramp Awning");
retained NCD8Relay rampPorch(ADDRESS1, NUMRELAYS, 3, "Ramp Porch");
retained NCD8Relay rearAwning(ADDRESS1, NUMRELAYS, 0, "Rear Awning");
retained NCD8Relay rearPorch(ADDRESS1, NUMRELAYS, 1, "Rear Porch");
//retained NCD8Relay rearAwningExtend(ADDRESS1, NUMRELAYS, 2, "Rear Awning Extend");
//retained NCD8Relay rearAwningRetract(ADDRESS1, NUMRELAYS, 1, "Rear Awning Retract");
//retained NCD8Relay rampAwningExtend(ADDRESS1, NUMRELAYS, 6, "Ramp Awning Extend");
//retained NCD8Relay rampAwningRetract(ADDRESS1, NUMRELAYS, 7, "Ramp Awning Retract");

//retained NCD8Light piano(ADDRESS2, 0, "Piano");

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("RearPanel");
    iot->begin();
    iot->connectMQTT(mqttServer, "PatriotRearPanel1", true);   // MQTT bridge enabled

    iot->addDevice(DEV_PTR loft);
    iot->addDevice(DEV_PTR office);
    iot->addDevice(DEV_PTR rampAwning);
    iot->addDevice(DEV_PTR rampPorch);
    iot->addDevice(DEV_PTR rearAwning);
    iot->addDevice(DEV_PTR rearPorch);
//    iot->addDevice(DEV_PTR rearAwningExtend);
//    iot->addDevice(DEV_PTR rearAwningRetract);
//    iot->addDevice(DEV_PTR rampAwningExtend);
//    iot->addDevice(DEV_PTR rampAwningRetract);

//    iot->addDevice(DEV_PTR piano);

    
    // BEHAVIORS
    // Arriving
    iot->addBehavior(new Behavior(DEV_PTR rampPorch, "arriving", '>', 0, 100));
    iot->addBehavior(new Behavior(DEV_PTR rearPorch, "arriving", '>', 0, 100));

    // Cleaning
    iot->addBehavior(new Behavior(DEV_PTR office, "cleaning", '>', 0, 100));
    iot->addBehavior(new Behavior(DEV_PTR loft,   "cleaning", '>', 0, 100));

    // Cooking

    // Evening - Porch lights on (awnings once we can detect if extended)
    iot->addBehavior(new Behavior(DEV_PTR rampPorch,  "evening", '>', 0, 100));
    iot->addBehavior(new Behavior(DEV_PTR rearPorch,  "evening", '>', 0, 100));

    // Everything (On/Off)
    iot->addBehavior(new Behavior(DEV_PTR office, "everything", '>', 0, 100));
    iot->addBehavior(new Behavior(DEV_PTR loft,   "everything", '>', 0, 100));
    iot->addBehavior(new Behavior(DEV_PTR rampPorch,  "everything", '>', 0, 100));
    iot->addBehavior(new Behavior(DEV_PTR rampAwning, "everything", '>', 0, 100));
    iot->addBehavior(new Behavior(DEV_PTR rearPorch,  "everything", '>', 0, 100));
    iot->addBehavior(new Behavior(DEV_PTR rearAwning, "everything", '>', 0, 100));
    iot->addBehavior(new Behavior(DEV_PTR office, "everything", '=', 0, 0));
    iot->addBehavior(new Behavior(DEV_PTR loft,   "everything", '=', 0, 0));
    iot->addBehavior(new Behavior(DEV_PTR rampPorch,  "everything", '=', 0, 0));
    iot->addBehavior(new Behavior(DEV_PTR rampAwning, "everything", '=', 0, 0));
    iot->addBehavior(new Behavior(DEV_PTR rearPorch,  "everything", '=', 0, 0));
    iot->addBehavior(new Behavior(DEV_PTR rearAwning, "everything", '=', 0, 0));

    // Hosting

    // Leaving
    iot->addBehavior(new Behavior(DEV_PTR office, "leaving", '>', 0, 0));
    iot->addBehavior(new Behavior(DEV_PTR loft,   "leaving", '>', 0, 0));

    // Morning
    iot->addBehavior(new Behavior(DEV_PTR office, "morning", '>', 0, 0));
    iot->addBehavior(new Behavior(DEV_PTR loft,   "morning", '>', 0, 0));
    iot->addBehavior(new Behavior(DEV_PTR rampPorch,  "morning", '>', 0, 0));
    iot->addBehavior(new Behavior(DEV_PTR rampAwning, "morning", '>', 0, 0));
    iot->addBehavior(new Behavior(DEV_PTR rearPorch,  "morning", '>', 0, 0));
    iot->addBehavior(new Behavior(DEV_PTR rearAwning, "morning", '>', 0, 0));

    // Reading

    // Retiring - everything off except bedroom and a few low lights
    iot->addBehavior(new Behavior(DEV_PTR office, "bedtime", '>', 0, 0));
    iot->addBehavior(new Behavior(DEV_PTR loft,   "bedtime", '>', 0, 0));
    iot->addBehavior(new Behavior(DEV_PTR rampPorch,  "bedtime", '>', 0, 0));
    iot->addBehavior(new Behavior(DEV_PTR rampAwning, "bedtime", '>', 0, 0));
    iot->addBehavior(new Behavior(DEV_PTR rearPorch,  "bedtime", '>', 0, 0));
    iot->addBehavior(new Behavior(DEV_PTR rearAwning, "bedtime", '>', 0, 0));

    // Sleeping - Everything off
    iot->addBehavior(new Behavior(DEV_PTR office, "sleeping", '>', 0, 0));
    iot->addBehavior(new Behavior(DEV_PTR loft,   "sleeping", '>', 0, 0));
    iot->addBehavior(new Behavior(DEV_PTR rampPorch,  "sleeping", '>', 0, 0));
    iot->addBehavior(new Behavior(DEV_PTR rampAwning, "sleeping", '>', 0, 0));
    iot->addBehavior(new Behavior(DEV_PTR rearPorch,  "sleeping", '>', 0, 0));
    iot->addBehavior(new Behavior(DEV_PTR rearAwning, "sleeping", '>', 0, 0));

    // Waking
    
    // Watching


}

void loop() {
    iot->loop();
}
