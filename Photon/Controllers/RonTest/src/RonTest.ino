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

String mqttServerIP = "192.168.10.184";

IoT     *iot;

Light testLed(2, "testLed");                // Not connected to anything
Light blueLed(7, "blueLed", false, true);

// Activities allow Alexa to control them
// and can also turn off other activities.
Activity waking("waking");
Activity watchingTV("watchingTV");
Activity goingToBed("goingToBed");
Activity sleeping("sleeping");

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("RonTest");
    iot->begin();
    iot->connectMQTT(mqttServerIP, "patriotRonTest1", false);
        
    // Behaviors/Activities
    blueLed.addBehavior(100, "waking", '>', 0);

    waking.setOtherState("sleeping", 0);    // Turn off sleeping when waking
    
    sleeping.setOtherState("waking", 0);    // Beware of loops

    goingToBed.setOtherState("watchingtv", 0);
    goingToBed.setOtherState("waking", 0);


    // Devices and Activities
    iot->addDevice(&blueLed);
    iot->addDevice(&testLed);

    iot->addDevice(&waking);
    iot->addDevice(&watchingTV);
    iot->addDevice(&goingToBed);
    iot->addDevice(&sleeping);
    
    Log.trace("Trace message");
    Log.info("Info message");
    Log.warn("Warn message");
    Log.error("Error message");
}

void loop() {
    iot->loop();
}
