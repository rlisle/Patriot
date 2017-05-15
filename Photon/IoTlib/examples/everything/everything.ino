/*******************************************************
Everything Patriot Example

Note: this example is not yet complete, but works with
      the devices that have been added so far.

This example contains one or more of each IoT device type
supported by Patriot. This includes the following:

 * 4 LED Lights
    - Red   = Outside       A3      Photon pin 9
    - Green = Kitchen       RX      Photon pin 4
    - Blue  = Bedroom       TX      Photon pin 3
    - Amber = Bathroom      WKP     Photon pin 5

 * 4 Switches
    - WakeUp                D4      Photon pin 17
    - WatchTV               D5      Photon pin 18
    - Cook                  D6      Photon pin 19
    - Sleep                 D7      Photon pin 20

 * Temperature & Humidity sensor
    - DHT11 or DHT22        A2      Photon pin 10

 * HC-SR04 Ultrasonic Proximity Sensor
    - Trigger               A1      Photon pin 11
    - Echo                  A0      Photon pin 12

 * Light level sensor
    - Photoresistor         A4      Photon pin 8

 * Fan                              (TODO:)
    - PWM output            DAC     Photon pin ??

 * Servo Motor                      (TODO:)
    - PWM output            A5      Photon pin ??

 * Stepper Motor                    (TODO:)
    - Stepper pin 1         D0      Photon pin 13
    - Stepper pin 2         D1      Photon pin 14
    - Stepper pin 3         D2      Photon pin 15
    - Stepper pin 4         D3      Photon pin 16

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-28: Use fixed 'patriot' event name.
2017-03-24: Rename Patriot
2017-03-11: TODO: add remaining devices.
2017-03-07: Initial creation
********************************************************/

#include <IoT.h>
#include "light.h"
#include "switch.h"

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("myPhoton");
    iot->begin();

    iot->addDevice(new Light(A3, "Outside"));
    iot->addDevice(new Light(RX, "Kitchen"));
    iot->addDevice(new Light(TX, "Livingroom"));
    iot->addDevice(new Light(WKP, "Bedroom"));

    iot->addDevice(new Switch(D4, "WakeUp"));
    iot->addDevice(new Switch(D5, "WatchTV"));
    iot->addDevice(new Switch(D6, "Cook"));
    iot->addDevice(new Switch(D7, "Sleep"));

    //TODO: Add light level sensor

    //TODO: Add Fan control

    //TODO: Add servo motor

    //TODO: Add stepper motor

    iot->addBehavior("Outside", new Behavior("WakeUp",'>',0, 100));
    iot->addBehavior("Kitchen", new Behavior("WatchTV",'>',0, 100));
    iot->addBehavior("Livingroom", new Behavior("Cook",'>',0, 100));
    iot->addBehavior("Bedroom", new Behavior("Sleep",'>',0, 100));

//    iot->monitorPresence(A1, A0, 3, 36, "Bathroom");
//    iot->monitorTemperature(A2, DHT22, "Kitchen", 5000);

//    iot->exposeControllers();
//    iot->exposeActivities();
}

void loop() {
    iot->loop();
}
