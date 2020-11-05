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
#include <PatriotNCD8Light.h>

#define DEV_PTR (Device *)&

String mqttServerIP = "192.168.10.184";

IoT     *iot;

// Use Backup SRAM to persist led state between resets
//retained Light led(D7, "led", false, true);
retained NCD8Light test1(1, 0, "test1", 0); // immediate
retained NCD8Light test2(1, 1, "test2", 1); // 1 second transition
retained NCD8Light test3(1, 2, "test3", 2); // 2 " "
retained NCD8Light test4(1, 3, "test4", 3);
retained NCD8Light test5(1, 0, "test5", 4);
retained NCD8Light test6(1, 1, "test6", 5);
retained NCD8Light test7(1, 2, "test7", 6);
retained NCD8Light test8(1, 3, "test8", 7);

void setup() {
  iot = IoT::getInstance();
  iot->setControllerName("RonTest");
  iot->begin();
  iot->connectMQTT(mqttServerIP, "patriotRonTest1");

  // test1.setLocalPin(A0, "Switch1");
  // test2.setLocalPin(A1, "Switch2");
  // test3.setLocalPin(A2, "Switch3");
  // test4.setLocalPin(A3, "Switch4");

  // Devices
//  iot->addDevice(DEV_PTR led);
  iot->addDevice(DEV_PTR test1);
  iot->addDevice(DEV_PTR test2);
  iot->addDevice(DEV_PTR test3);
  iot->addDevice(DEV_PTR test4);
  iot->addDevice(DEV_PTR test5);
  iot->addDevice(DEV_PTR test6);
  iot->addDevice(DEV_PTR test7);
  iot->addDevice(DEV_PTR test8);

  // Behaviors/Activities
  // iot->addBehavior(new Behavior(DEV_PTR light, "demo", '>', 0, 100));
  // iot->addBehavior(new Behavior(DEV_PTR light, "demo", '=', 0, 0));

  // Watchdog Timer
//  PhotonWdgs::begin(true,true,10000,TIMER7);
}

void loop() {
  // call tickle regularly from your code, to ensure the watchdogs do not reset
//  PhotonWdgs::tickle();

  iot->loop();
}
