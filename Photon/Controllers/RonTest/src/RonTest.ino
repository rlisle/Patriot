/*
 * RonTest Controller
 *
 * Description: This sketch provides the bridge, PartOfDay, and Activities
 *
 * Author: Ron Lisle
 *
 * Hardware
 * - built-in blue LED     D7
 */

#include <IoT.h>
#include <PatriotLight.h>
#include <PatriotActivity.h>
#include <PatriotPartOfDay.h>

String mqttServerIP = "192.168.10.184";

IoT     *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("RonTest");
    iot->begin();
    iot->connectMQTT(mqttServerIP, "patriotRonTest1", true);    // Is bridge

    iot->addDevice(new PartOfDay());

    iot->addDevice(new Light(7, "blueLed", false, true));

    // Activities allow Alexa to control them
    // and can also turn off other activities.
    iot->addDevice(new Activity("cooking"));
    iot->addDevice(new Activity("cleaning"));
    iot->addDevice(new Activity("sleeping"));
}

void loop() {
    iot->loop();
}
