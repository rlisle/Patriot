/*
 * Wall Switch Controller
 *
 * Description: This sketch controls the wall switches next to the booth.
 * Author: Ron Lisle
 *
 * It functions as the master controller; exposing the 'CurrentActivities'
 * variable to track all currently active activities.
 *
 * This module was selected as the master controller because it does
 * not contain any behaviors, nor support any activities of its own.
 *
 * Note: previously a Controllers variable was exposed that tracked
 *       online controllers based on Alive messages. This has been
 *       deprecated. The particle.io API is used instead to determine
 *       which controllers are online. The variable is still exposed,
 *       but not used by the apps.
 *
 * 1. Read wall switches and publish changes.
 * 2. Monitor alive messages, and expose 'controllers' variable.
 * 3. Monitor activity events and expose 'activities' variable.
 *
 * Hardware
 * 1. Photon
 * 2. 5x5min Photon IoTv2
 * 3. 4x wall switches (Stepper 1: D4, D5, D6, D7)
 * 4. PIR Motion Sensor (LED 2 / A5)
 *
 * Wiring
 * 1. 5 wire thermostat cable to behind refrigerator
 *      Currently providing power to Fan controller
 *        Read  +12
 *        Blue  Gnd
 *
 * History
 *  5/27/17 Update to plugins
 *  3/16/17 Convert to IoT library
 *  1/28/17 Expose activities variable
 *  1/10/17 Add master controller support (controllers variable)
 * 12/07/16 Update to current IoT
 *  8/12/16 Change switches to send events instead of "BoothSw#"
 */
#include <IoT.h>
#include <PatriotSwitch.h>

IoT*     iot;

void setup() {

  iot = IoT::getInstance();
  iot->setControllerName("BoothSwitch");
  iot->setPublishName("patriot");
  iot->begin();

  iot->exposeControllers();
  iot->exposeActivities();

  pinMode(D7, OUTPUT);
  digitalWrite(D7, LOW);

  Switch *switch1 = new Switch(D6,"computer"); // Right-most switch
    Switch *switch2 = new Switch(D5,"booth");
    Switch *switch3 = new Switch(D4,"all");
    Switch *switch4 = new Switch(D3,"outside");    // Left-most switch

    iot->addDevice(switch1);
    iot->addDevice(switch2);
    iot->addDevice(switch3);
    iot->addDevice(switch4);

//  iot->monitorPresence(kTriggerPin, kEchoPin, 3, 36, "piano");  // A1, A0
}

void loop() {
  iot->loop();
}
