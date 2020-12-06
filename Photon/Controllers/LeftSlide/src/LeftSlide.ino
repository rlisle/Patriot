/**
Left Slide Controller
Description: This sketch controls the 2 lights and switch in the left slide.
Author: Ron Lisle
 
  To update Photon:
    1. Edit this code
    2. Update IoT and plugins if needed
    3. "particle flash LeftSlide"
 
 Hardware
 1. NCD Photon Screw Terminal board
    I/Os selected to all be on the same side
     2 switch connections:
       A0 Watch TV
       A1 Reading
    2 LED driver boards
       TX Vertical Lights
       RX Reading lights
 */
#include <IoT.h>
#include <PatriotLight.h>
#include <PatriotSwitch.h>
#include <PatriotActivity.h>

String mqttServer = "192.168.1.10";

IoT *iot;

Light couch(TX, "Couch");
Light vertical(RX, "LeftVertical");

void setup() {
  iot = IoT::getInstance();
  iot->setControllerName("LeftSlide");
  iot->begin();
  iot->connectMQTT(mqttServer, "patriotLeftSlide");

  couch.setLocalPin(A0, "Watching");
  vertical.setLocalPin(A1, "Reading");

  // Devices
  iot->addDevice(DEV_PTR couch);
  iot->addDevice(DEV_PTR vertical);

  // BEHAVIORS
  // Note that ON is required, but OFF is optional

  // Arriving
  //iot->addBehavior(new Behavior(DEV_PTR vertical,  "arriving", '>', 0, 50));

  // Cleaning (On/Off)
  // iot->addBehavior(new Behavior(DEV_PTR couch, "cleaning", '>', 0, 100));

  // Cooking (On/Off)

  // Evening - Porch lights on (awnings once we can detect if extended)
//  iot->addBehavior(new Behavior(DEV_PTR vertical,  "evening", '>', 0, 50));

  // Everything (on/off)
  // iot->addBehavior(new Behavior(DEV_PTR couch, "everything", '>', 0, 100));     // On
  // iot->addBehavior(new Behavior(DEV_PTR vertical, "everything", '>', 0, 100));    // On

  // Hosting (was guest mode)

  // Leaving
  // iot->addBehavior(new Behavior(DEV_PTR couch,  "leaving", '>', 0, 0));
  // iot->addBehavior(new Behavior(DEV_PTR vertical, "leaving", '>', 0, 0));

  // Morning
  // Turn off any lights turned on my Waking
  // iot->addBehavior(new Behavior(DEV_PTR vertical,  "morning", '>', 0, 0));

  // Reading (on/off)
  iot->addBehavior(new Behavior(DEV_PTR couch,  "reading", '>', 0, 100));

  // Retiring - everything except bedroom off and trim
  // iot->addBehavior(new Behavior(DEV_PTR couch,  "retiring", '>', 0, 0));
  // iot->addBehavior(new Behavior(DEV_PTR vertical, "retiring", '>', 0, 0));

  // Sleeping - Everything off
  iot->addBehavior(new Behavior(DEV_PTR couch,  "sleeping", '>', 0, 0));
  iot->addBehavior(new Behavior(DEV_PTR vertical, "sleeping", '>', 0, 0));

  // Waking
  // iot->addBehavior(new Behavior(DEV_PTR vertical,  "waking", '>', 0, 2));

  // Watching
  iot->addBehavior(new Behavior(DEV_PTR couch,  "watching", '>', 0, 30));
  iot->addBehavior(new Behavior(DEV_PTR vertical, "watching", '>', 0, 30));
}

void loop() {
  iot->loop();
}
