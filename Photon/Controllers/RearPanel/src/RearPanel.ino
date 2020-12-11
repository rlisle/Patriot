/**
RearPanel Controller
Description: This sketch controls all the switches in the Cyclone 4005 rear control panel.
Author: Ron Lisle
 
 This is the bridge controller, so all activities should be added here.
 This controller will then repeat them to MQTT.
 
  To update Photon:
    1. Edit this code
    2. Update IoT and plugins if needed
    3. "particle flash RearPanel"
 
 Hardware
 1. NCD Photon Screw Terminal board
     6 switch connections:
       A0 Ceiling (brown)
       A1 Loft (red)
       A2 Ramp Porch Floods (yellow)
       A3 Ramp Awning LEDs (green)
       A4 Rear Porch Flood (blue)
       A5 Rear Awning LEDs (white)
  2. NCD 8 PWM OC 8W I2C Dimmer board
       I/O 0 Ceiling
       I/O 1 Loft
       I/O 2 Ramp Porch Floods
       I/O 3 Ramp Awning LEDs
       I/O 4 Rear Porch Flood
       I/O 5 Rear Awning LEDs
       I/O 6 Piano Spot
       I/O 7 ?

  Other
   - built-in blue LED     D7
 
 */
#include <IoT.h>
#include <PatriotSwitch.h>
#include <PatriotNCD8Light.h>
#include <PatriotActivity.h>

#define ADDRESS 1   // PWM board address A0 jumper set

String mqttServer = "192.168.10.184";

IoT *iot;

NCD8Light ceiling(ADDRESS, 0, "OfficeCeiling", 2);
NCD8Light loft(ADDRESS, 1, "Loft", 2);
NCD8Light rampPorch(ADDRESS, 2, "RampPorch", 2);
NCD8Light rampAwning(ADDRESS, 3, "RampAwning", 2);
NCD8Light rearPorch(ADDRESS, 4, "RearPorch", 2);
NCD8Light rearAwning(ADDRESS, 5, "RearAwning", 2);
NCD8Light piano(ADDRESS, 6, "Piano", 2);
// one unused dimmer I/O

// Switch control functional sets of lights, not individual lights
Switch officeSwitch(A0, "OfficeSwitch");
Switch loftSwitch(A1, "LoftSwitch");
Switch wakingSwitch(A2, "WakingSwitch");
Switch awningSwitch(A3, "AwningSwitch");
Switch floodsSwitch(A4, "FloodsSwitch");
Switch pianoSwitch(A5, "PianoSwitch");
// More available inputs A6, A7, TX, RX - use for door switch, motion detector, etc.

// Activities allow Alexa to control them
// and can also turn off other activities.
// These will be used by other panels also, but don't need to be duplicated by them
Activity waking("waking");                  // Turns off sleeping
Activity watchingTV("watchingTV");
Activity goingToBed("goingToBed");          // Turns off waking and others
Activity sleeping("sleeping");              // Turns off goingToBed, waking and others
Activity playingPiano("playingPiano");

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("RearPanel");
    iot->begin();
    iot->connectMQTT(mqttServer, "PatriotRearPanel1", true);   // MQTT bridge enabled

    // Set other states
    waking.setOtherState("sleeping", 0);        // Turn off sleeping when waking
    waking.setOtherState("goingtobed", 0);      // and goingToBed

    sleeping.setOtherState("waking", 0);        // Turn off waking when sleeping
    sleeping.setOtherState("watchingtv", 0);    // and watchingTV
    sleeping.setOtherState("goingtobed", 0);    // and goingToBed
    sleeping.setOtherState("playingpiano", 0);  // and playingPiano

    goingToBed.setOtherState("watchingtv", 0);  // Turn off watchingTV when going to bed
    goingToBed.setOtherState("playingpiano", 0);// and playingPiano
    goingToBed.setOtherState("waking", 0);      // and waking

    // BEHAVIORS
    ceiling.addBehavior(30, "waking", '>', 0);
    piano.addBehavior(100, "playingpiano", '>', 0);

    // Switches
    ceiling.addBehavior(100, "OfficeSwitch", '>', 0);
    loft.addBehavior(100, "LoftSwitch", '>', 0);
    rampAwning.addBehavior(100, "AwningSwitch", '>', 0);
    rearAwning.addBehavior(100, "AwningSwitch", '>', 0);
    rampPorch.addBehavior(100, "FloodSwitch", '>', 0);
    rearPorch.addBehavior(100, "FloodSwitch", '>', 0);
    piano.addBehavior(100, "playingSwitch", '>', 0);

    // ADD ALL DEVICES
    iot->addDevice(&ceiling);
    iot->addDevice(&loft);
    iot->addDevice(&rampPorch);
    iot->addDevice(&rampAwning);
    iot->addDevice(&rearPorch);
    iot->addDevice(&rearAwning);
    iot->addDevice(&piano);

    iot->addDevice(&officeSwitch);
    iot->addDevice(&loftSwitch);
    iot->addDevice(&wakingSwitch);
    iot->addDevice(&awningSwitch);
    iot->addDevice(&floodsSwitch);
    iot->addDevice(&pianoSwitch);
    
    iot->addDevice(&waking);
    iot->addDevice(&watchingTV);
    iot->addDevice(&goingToBed);
    iot->addDevice(&sleeping);
    iot->addDevice(&playingPiano);
}

void loop() {
    iot->loop();
}
