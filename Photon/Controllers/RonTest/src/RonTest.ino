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
 * - built-in blue LED     D7
 * - 4 Switches on A0-A3
 * - 4 LEDs on D0-D3
 *
 * History
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
//#include <PatriotNCD8Light.h>
#include <PatriotLight.h>

//#define DEV_PTR (Device *)&

String mqttServerIP = "192.168.10.184";

IoT     *iot;

// Use Backup SRAM to persist led state between resets
// To use persistent storage, insert "retained" before NCD8Relay
//Light led(D7, "led", false, true);
// NCD8Light test1(1, 0, "test1", 0); // immediate
// NCD8Light test2(1, 1, "test2", 1); // 1 second transition
// NCD8Light test3(1, 2, "test3", 2); // 2 " "
// NCD8Light test4(1, 3, "test4", 3);
// NCD8Light test5(1, 4, "test5", 4);
// NCD8Light test6(1, 5, "test6", 5);
// NCD8Light test7(1, 6, "test7", 6);
// NCD8Light test8(1, 7, "test8", 7);
Light test(7, "test", false, true);

void setup() {
  iot = IoT::getInstance();
  iot->setControllerName("RonTest");
  iot->begin();
  iot->connectMQTT(mqttServerIP, "patriotRonTest1");

  // test1.setLocalPin(A0, "Switch1");
  // test2.setLocalPin(A1, "Switch2");
  // test3.setLocalPin(A2, "Switch3");
  // test4.setLocalPin(A3, "Switch4");

  // Behaviors/Activities
  Behavior *demo = new Behavior(100);
  demo->addCondition(new Condition("demo", '>', 0));
  test.addBehavior(demo);

  // Devices
//  iot->addDevice(DEV_PTR led);
  iot->addDevice(&test);

  // Watchdog Timer
//  PhotonWdgs::begin(true,true,10000,TIMER7);
}

void loop() {
  // call tickle regularly from your code, to ensure the watchdogs do not reset
//  PhotonWdgs::tickle();

  iot->loop();
}
