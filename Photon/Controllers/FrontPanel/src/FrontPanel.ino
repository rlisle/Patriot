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
   - ?Vent Open            (4)
   - ?Vent Close           (5)
 * I2C 8x dimmer board connections 0x21
   - Ceiling              (5)
   - Sink                 (1)
   - High Kitchen         (1)
   - Kitchen Ceiling      (2)
   - ?Vent Fan             (3)
   - Top step
   - Bottom step
   - Hall ceiling
 * Activities
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
//#include <PatriotFan.h>
//#include <PatriotMotorized.h>

#define DEV_PTR (Device *)&

IoT *iot;

//String mqttServer = "rons-mac-mini";
String mqttServer = "192.168.10.184";

#define ADDRESS1 0x20
#define ADDRESS2 0x21
#define NUMRELAYS 8

retained NCD8Relay kitchenSink(ADDRESS1, NUMRELAYS, 0, "Sink");
retained NCD8Relay frontAwning(ADDRESS1, NUMRELAYS, 1, "FrontAwning");
retained NCD8Relay rightTrim(ADDRESS1, NUMRELAYS, 2, "RightTrim");
retained NCD8Relay leftTrim(ADDRESS1, NUMRELAYS, 3, "LeftTrim");
retained NCD8Relay ceiling(ADDRESS1, NUMRELAYS, 4, "Ceiling");
retained NCD8Relay dsFloods(ADDRESS1, NUMRELAYS, 5, "DoorSide");
retained NCD8Relay osFloods(ADDRESS1, NUMRELAYS, 6, "OtherSide");
retained NCD8Relay frontPorch(ADDRESS1, NUMRELAYS, 7, "FrontPorch");

//    NCD8Relay *highKitchen          = new NCD8Relay(ADDRESS2, numRelays2, 0, "HighKitchen");
//    NCD8Relay *kitchenCeiling       = new NCD8Relay(ADDRESS2, numRelays2, 1, "KitchenCeiling");
//    NCD8Relay *ventFan              = new NCD8Relay(ADDRESS2, numRelays2, 2, "VentFan");
//    NCD8Relay *ventOpen             = new NCD8Relay(ADDRESS2, numRelays2, 3, "VentOpen", 5);
//    NCD8Relay *ventClose            = new NCD8Relay(ADDRESS2, numRelays2, 4, "VentClose", 5);

    // TODO: Fan
//    Fan *ventFan = new Fan(D4, "ventFan");

    // TODO: motorized vent & awning
//    Motorized *vent = new Motorized(D5, D6, 30000, "vent");         //TODO: set actual duration
//    Motorized *frontAwning = new Motorized(RX, TX, 60000, "vent");  //TODO: set actual duration

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("FrontPanel");
    iot->begin();
    iot->connectMQTT(mqttServer, "patriotFrontPanel1");

    // DEVICES

    iot->addDevice(DEV_PTR ceiling);
    iot->addDevice(DEV_PTR dsFloods);
    iot->addDevice(DEV_PTR frontAwning);
    iot->addDevice(DEV_PTR frontPorch);
    iot->addDevice(DEV_PTR kitchenSink);
    iot->addDevice(DEV_PTR leftTrim);
    iot->addDevice(DEV_PTR osFloods);
    iot->addDevice(DEV_PTR rightTrim);

//    iot->addDevice(DEV_PTR highKitchen);
//    iot->addDevice(DEV_PTR kitchenCeiling);
//    iot->addDevice(DEV_PTR ventFan);            // TODO: convert to Fan device
//    iot->addDevice(DEV_PTR ventOpen);           // TODO: convert to motorized vent
//    iot->addDevice(DEV_PTR ventClose);          // TODO: "

    // BEHAVIORS
    // Note that ON is required, but OFF is optional

    // Arriving (currently done in HomeKit)
    // iot->addBehavior(new Behavior(DEV_PTR frontPorch, "arriving", '>', 0, 100));
    // iot->addBehavior(new Behavior(DEV_PTR leftTrim, "arriving", '>', 0, 100));
    // iot->addBehavior(new Behavior(DEV_PTR rightTrim, "arriving", '>', 0, 100));

    // Cleaning  (on/off)
    // iot->addBehavior(new Behavior(DEV_PTR ceiling, "cleaning", '>', 0, 100));
    // iot->addBehavior(new Behavior(DEV_PTR ceiling, "cleaning", '=', 0, 0));
    // iot->addBehavior(new Behavior(DEV_PTR kitchenSink,   "cleaning", '>', 0, 100));
    // iot->addBehavior(new Behavior(DEV_PTR kitchenSink,   "cleaning", '=', 0, 0));

    // Cooking (On/Off)
    // iot->addBehavior(new Behavior(DEV_PTR kitchenSink, "cooking", '>', 0, 100));
    // iot->addBehavior(new Behavior(DEV_PTR kitchenSink, "cooking", '=', 0, 0));

    // Evening - Porch lights on (awnings once we can detect if extended)
    // iot->addBehavior(new Behavior(DEV_PTR frontPorch,  "evening", '>', 0, 100));
    // iot->addBehavior(new Behavior(DEV_PTR leftTrim,  "evening", '>', 0, 100));
    // iot->addBehavior(new Behavior(DEV_PTR rightTrim,  "evening", '>', 0, 100));

    // Everything (on/off)
    // iot->addBehavior(new Behavior(DEV_PTR ceiling, "everything", '>', 0, 100));     // On
    // iot->addBehavior(new Behavior(DEV_PTR ceiling, "everything", '=', 0, 0));       // Off
    // iot->addBehavior(new Behavior(DEV_PTR dsFloods, "everything", '>', 0, 100));    // On
    // iot->addBehavior(new Behavior(DEV_PTR dsFloods, "everything", '=', 0, 0));      // Off
    // iot->addBehavior(new Behavior(DEV_PTR frontAwning, "everything", '>', 0, 100)); // On
    // iot->addBehavior(new Behavior(DEV_PTR frontAwning, "everything", '=', 0, 0));   // Off
    // iot->addBehavior(new Behavior(DEV_PTR frontPorch, "everything", '>', 0, 100)); // On
    // iot->addBehavior(new Behavior(DEV_PTR frontPorch, "everything", '=', 0, 0));   // Off
    // iot->addBehavior(new Behavior(DEV_PTR kitchenSink, "everything", '>', 0, 100));       // on
    // iot->addBehavior(new Behavior(DEV_PTR kitchenSink, "everything", '=', 0, 0));         // Off
    // iot->addBehavior(new Behavior(DEV_PTR leftTrim, "everything", '>', 0, 100));    // On
    // iot->addBehavior(new Behavior(DEV_PTR leftTrim, "everything", '=', 0, 0));      // Off
    // iot->addBehavior(new Behavior(DEV_PTR osFloods, "everything", '>', 0, 100));    // On
    // iot->addBehavior(new Behavior(DEV_PTR osFloods, "everything", '=', 0, 0));      // Off
    // iot->addBehavior(new Behavior(DEV_PTR rightTrim, "everything", '>', 0, 100));   // On
    // iot->addBehavior(new Behavior(DEV_PTR rightTrim, "everything", '=', 0, 0));     // Off

    // Hosting (on/off - was guest mode)

    // Leaving
    // iot->addBehavior(new Behavior(DEV_PTR kitchenSink, "leaving", '>', 0, 0));

    // Morning
    // Undo any lights turned on by Waking
    //iot->addBehavior(new Behavior(DEV_PTR rightTrim,  "morning", '>', 0, 0));

    // Reading
    // iot->addBehavior(new Behavior(DEV_PTR leftTrim, "reading", '>', 0, 100));

    // Retiring - everything off except bedroom and a few low lights
    // iot->addBehavior(new Behavior(DEV_PTR ceiling,  "bedtime", '>', 0, 0));
    // iot->addBehavior(new Behavior(DEV_PTR dsFloods, "bedtime", '>', 0, 0));
    // iot->addBehavior(new Behavior(DEV_PTR frontAwning, "bedtime", '>', 0, 0));
    // iot->addBehavior(new Behavior(DEV_PTR frontPorch, "bedtime", '>', 0, 0));
    // iot->addBehavior(new Behavior(DEV_PTR kitchenSink, "bedtime", '>', 0, 0));
    // iot->addBehavior(new Behavior(DEV_PTR leftTrim, "bedtime", '>', 0, 100));
    // iot->addBehavior(new Behavior(DEV_PTR osFloods, "bedtime", '>', 0, 0));
    // iot->addBehavior(new Behavior(DEV_PTR rightTrim, "bedtime", '>', 0, 100));

    // Sleeping - Everything off
    iot->addBehavior(new Behavior(DEV_PTR ceiling,  "sleeping", '>', 0, 0));
    iot->addBehavior(new Behavior(DEV_PTR dsFloods, "sleeping", '>', 0, 0));
    iot->addBehavior(new Behavior(DEV_PTR frontAwning, "sleeping", '>', 0, 0));
    iot->addBehavior(new Behavior(DEV_PTR frontPorch, "sleeping", '>', 0, 0));
    iot->addBehavior(new Behavior(DEV_PTR kitchenSink, "sleeping", '>', 0, 0));
    iot->addBehavior(new Behavior(DEV_PTR leftTrim, "sleeping", '>', 0, 0));
    iot->addBehavior(new Behavior(DEV_PTR osFloods, "sleeping", '>', 0, 0));
    iot->addBehavior(new Behavior(DEV_PTR rightTrim, "sleeping", '>', 0, 0));

    // Waking
    //iot->addBehavior(new Behavior(DEV_PTR rightTrim, "waking", '>', 0, 100));

    // Watching (TV)
    // iot->addBehavior(new Behavior(DEV_PTR leftTrim, "watching", '>', 0, 100));
    // iot->addBehavior(new Behavior(DEV_PTR ceiling,  "watching", '>', 0, 0));
    // iot->addBehavior(new Behavior(DEV_PTR kitchenSink, "watching", '>', 0, 0));
}

void loop() {
    iot->loop();
}
