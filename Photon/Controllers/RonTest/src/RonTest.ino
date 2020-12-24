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

    PartOfDay *partOfDay = new PartOfDay();

    Light *testLed = new Light(2, "testLed");   // Not connected
    Light *blueLed = new Light(7, "blueLed", false, true);

    // Activities allow Alexa to control them
    // and can also turn off other activities.
    Activity *cooking = new Activity("cooking");
    Activity *cleaning = new Activity("cleaning");

//    // Behaviors/Activities
//    blueLed->addBehavior(100, "cooking", '>', 0);
//    blueLed->addBehavior(100, "cleaning", '>', 0);
//
//    cleaning->setOtherState("cooking", 0);
//    cooking->setOtherState("cleaning", 0);
    

    // Devices and Activities
    iot->addDevice(blueLed);
    iot->addDevice(testLed);
    iot->addDevice(partOfDay);

    iot->addDevice(cooking);
    iot->addDevice(cleaning);
    
//    Log.error("Error message");
//    Log.warn("Warn message");
//    Log.info("Info message");
//    Log.trace("Trace message");

//    Log(LOG_LEVEL_ERROR, "LOG_LEVEL_ERROR = " + String(LOG_LEVEL_ERROR));
//    Log(LOG_LEVEL_ERROR, "LOG_LEVEL_WARN = " + String(LOG_LEVEL_WARN));
//    Log(LOG_LEVEL_ERROR, "LOG_LEVEL_INFO = " + String(LOG_LEVEL_INFO));
//    Log(LOG_LEVEL_ERROR, "LOG_LEVEL_TRACE = " + String(LOG_LEVEL_TRACE));
//    Log(LOG_LEVEL_ERROR, "LOG_LEVEL_NONE = " + String(LOG_LEVEL_NONE));
//    Log(LOG_LEVEL_ERROR, "LOG_LEVEL_ALL = " + String(LOG_LEVEL_ALL));
}

void loop() {
    iot->loop();
}
