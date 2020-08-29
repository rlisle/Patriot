/*
 * LeftSlide Controller
 * Description: This sketch controls both lights in the left slide.
 * Author: Ron Lisle
 * Date: 11/3/19
 *
 * To update Photon:
 *   1. Edit this code
 *   2. "particle flash LeftSlide"
 *
 * Hardware
 * 1. Photon
 * 2. 3 x LED boards: RX, TX, WKP
 * 3. 2 x switch inputs: A0, A1
 *
 * Activities
   - arriving
   - cleaning
   - cooking
   - evening / sunsetting
   - everything (all lights)
   - hosting
   - leaving
   - morning / sunrising
   - reading 
   - retiring (bedtime)
   - sleeping
   - waking
   - watching
 *
 * History
 * 11/03/19 Initial creation
 */
#include <IoT.h>
#include <PatriotLight.h>

#define DEV_PTR (Device *)&

IoT *iot;

String mqttServer = "192.168.1.10";

// Use Backup SRAM to persist led state between resets
retained Light couch(TX, "Couch");
retained Light vertical(RX, "Left Vertical");

void setup() {
  iot = IoT::getInstance();
  iot->setControllerName("LeftSlide");
  iot->begin();
  iot->connectMQTT(mqttServer, "patriotLeftSlide");

  couch.setLocalPin(A0, "Movies");
  vertical.setLocalPin(A1, "Reading");

  // Devices
  iot->addDevice(DEV_PTR couch);
  iot->addDevice(DEV_PTR vertical);

  // BEHAVIORS
  // Note that ON is required, but OFF is optional

  // Arriving
  iot->addBehavior(new Behavior(DEV_PTR vertical,  "arriving", '>', 0, 50));

  // Cleaning (On/Off)
  iot->addBehavior(new Behavior(DEV_PTR couch, "cleaning", '>', 0, 100));
  iot->addBehavior(new Behavior(DEV_PTR couch, "cleaning", '=', 0, 0));

  // Cooking (On/Off)

  // Evening - Porch lights on (awnings once we can detect if extended)
  iot->addBehavior(new Behavior(DEV_PTR vertical,  "evening", '>', 0, 50));

  // Everything (on/off)
  iot->addBehavior(new Behavior(DEV_PTR couch, "everything", '>', 0, 100));     // On
  iot->addBehavior(new Behavior(DEV_PTR couch, "everything", '=', 0, 0));       // Off
  iot->addBehavior(new Behavior(DEV_PTR vertical, "everything", '>', 0, 100));    // On
  iot->addBehavior(new Behavior(DEV_PTR vertical, "everything", '=', 0, 0));      // Off

  // Hosting (was guest mode)

  // Leaving
  iot->addBehavior(new Behavior(DEV_PTR couch,  "leaving", '>', 0, 0));
  iot->addBehavior(new Behavior(DEV_PTR vertical, "leaving", '>', 0, 0));

  // Morning
  // Turn off any lights turned on my Waking
  iot->addBehavior(new Behavior(DEV_PTR vertical,  "morning", '>', 0, 0));

  // Reading (on/off)
  iot->addBehavior(new Behavior(DEV_PTR couch,  "reading", '>', 0, 100));
  iot->addBehavior(new Behavior(DEV_PTR couch,  "reading", '=', 0, 0));

  // Retiring - everything except bedroom off and trim
  iot->addBehavior(new Behavior(DEV_PTR couch,  "retiring", '>', 0, 0));
  iot->addBehavior(new Behavior(DEV_PTR vertical, "retiring", '>', 0, 0));

  // Sleeping - Everything off
  iot->addBehavior(new Behavior(DEV_PTR couch,  "sleeping", '>', 0, 0));
  iot->addBehavior(new Behavior(DEV_PTR vertical, "sleeping", '>', 0, 0));

  // Waking
  iot->addBehavior(new Behavior(DEV_PTR vertical,  "waking", '>', 0, 2));

  // Watching
  iot->addBehavior(new Behavior(DEV_PTR couch,  "watching", '>', 0, 30));
  iot->addBehavior(new Behavior(DEV_PTR vertical, "watching", '>', 0, 30));
}

void loop() {
  iot->loop();
}
