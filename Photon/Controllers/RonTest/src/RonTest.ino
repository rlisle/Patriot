/*
 * RonTest Controller
 *
 * Description: This sketch provides a test environment for Patriot testing.
 * Author: Ron Lisle
 * Date: 10/09/18
 *
 * It simulates a controller using the built-in LED for testing.
 *
 * Currently using to develop NCD16Switch and Activity
 *
 * Hardware
 * - NCD Photon SCT board
 * - built-in blue LED     D7
 * - NCD 23017 16 GPIO board - no jumpers set
 *
 * History
 * 11/26/20 Testing NCD16Switch plugin and Activity plugin
 * 11/5/20  Testing NCD8Light plugin
 * 11/20/19 Testing Light brightness
 * 01/05/19 Remove watchdog timer due to OTA issues.
 * 01/03/19 Save device state in Backup SRAM
 * 12/31/18 Add hardware watchdog timer and dependency SparkIntervalTimer
 * 10/19/18 Remove Desk Lamp and behaviors.
 * 10/15/18 Add Desk Lamp STLight
 * 10/09/18 Update to Patriot 2.3.0 for ping/pong tests
 *  6/21/18 Update to Patriot 2.2.0 with MQTT
 *  3/16/17 Convert to IoT library
 *  2/11/17 Create initial file based on existing template.
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
    iot->setLogLevel(LogDebug);
        
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
}

void loop() {
    iot->loop();
}
