/*
 * RonTest Controller
 *
 * Description: This sketch provides a test environment for Patriot testing.
 * Author: Ron Lisle
 * Date: 10/09/18
 *
 * It simulates a controller using the built-in LED for testing.
 *
 * Hardware
 * - NCD Photon SCT board
 * - built-in blue LED     D7
 *
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
    iot->connectMQTT(mqttServerIP, "patriotRonTest1", false);

    iot->addDevice(new PartOfDay());

    iot->addDevice(new Light(2, "testLed"));        // Not connected to anything
    iot->addDevice(new Light(7, "blueLed", false, true));

    // Activities allow Alexa to control them
    // and can also turn off other activities.
    iot->addDevice(new Activity("cooking"));
    iot->addDevice(new Activity("cleaning"));
}

void loop() {
    iot->loop();
}
