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

// Switch control functional sets of lights, not individual lights
Switch couchSwitch1(A0, "CouchSwitch1");
Switch couchSwitch2(A1, "CouchSwitch2");

void setup() {
  iot = IoT::getInstance();
  iot->setControllerName("LeftSlide");
  iot->begin();
  iot->connectMQTT(mqttServer, "patriotLeftSlide");

  // Devices
  iot->addDevice(&couch);
  iot->addDevice(&vertical);
  iot->addDevice(&couchSwitch1);
  iot->addDevice(&couchSwitch2);

}

void loop() {
  iot->loop();
}
