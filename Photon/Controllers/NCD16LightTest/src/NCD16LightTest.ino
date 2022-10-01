/*
 * NCD16LightTest
 * Description: Turn on a PWM light
 * Author: Ron Lisle
 * Date: 10/1/22
 *
 * To update Photon:
 *   1. Edit this code
 *   2. "particle flash OfficeTest"
 *
 * Hardware
 * 1. Photon
 * 2. ControlEverything.com NCD Photon Screw Terminal Breakout board
 * 3. NCD16PWM board connected via I2C to NCD STB board
 * NCD 8-channel PWM OC board with adress switches set per ADDRESS
 * LED connected to channel 0
 * 
 * Note that the PCA9634 connects to 16 BUK98150-55A power FETS.
 * !OE is hardwired to ground, so OE always == 0 (outputs enabled)
 *
 * History
 * 10/1/22 Initial creation
 */

#include <IoT.h>
#include <PatriotNCD16Light.h>

#define CONTROLLER_NAME "OfficeTest"
#define MQTT_BROKER "192.168.50.33"
#define ADDRESS 1      // PWM board switches low switch on

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(SEMI_AUTOMATIC);

void setup() {
    WiFi.selectAntenna(ANT_INTERNAL);
    WiFi.useDynamicIP();
    IoT::begin(MQTT_BROKER, CONTROLLER_NAME);
    createDevices();
}

void CreateDevices() {
    Device::add(new NCD16Light(ADDRESS, 0, "SpotLight", "Office", 2))
}

void loop() {
    IoT::Loop();
}
