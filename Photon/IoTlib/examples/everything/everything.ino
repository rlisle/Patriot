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
#include <PatriotLight.h>
#include <PatriotSwitch.h>
#include <PatriotFan.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("myPhoton");
    iot->begin();

    // Create devices
    Light *outside = new Light(A3, "Outside");
    Light *kitchen = new Light(RX, "Kitchen");
    Light *living = new Light(TX, "Livingroom");
    Light *bedroom = new Light(WKP, "Bedroom");

    Switch *switch1 = new Switch(D4, "WakeUp");
    Switch *switch2 = new Switch(D5, "WatchTV");
    Switch *switch3 = new Switch(D6, "Cook");
    Switch *switch4 = new Switch(D7, "Sleep");

    Fan *fan = new Fan(DAC, "Fan");

    // Add them
    iot->addDevice(outside);
    iot->addDevice(kitchen);
    iot->addDevice(living);
    iot->addDevice(bedroom);

    iot->addDevice(switch1);
    iot->addDevice(switch2);
    iot->addDevice(switch3);
    iot->addDevice(switch4);

    iot->addDevice(fan);

    //TODO: Add light level sensor

    //TODO: Add servo motor

    //TODO: Add stepper motor

    // Setup behaviors for our devices
    iot->addBehavior(new Behavior(outside, "WakeUp", '>', 0, 100));
    iot->addBehavior(new Behavior(kitchen, "WatchTV", '>', 0, 100));
    iot->addBehavior(new Behavior(living, "Cook", '>', 0, 100));
    iot->addBehavior(new Behavior(bedroom, "Sleep",'>',0, 100));

    iot->addBehavior(new Behavior(fan, "InsideTemp", '>', 75, 10));
    iot->addBehavior(new Behavior(fan, "InsideTemp", '>', 80, 50));
    iot->addBehavior(new Behavior(fan, "InsideTemp", '>', 85, 100));

//    iot->monitorPresence(A1, A0, 3, 36, "Bathroom");

//    iot->exposeControllers();
//    iot->exposeActivities();
}

void loop() {
    iot->loop();
}
