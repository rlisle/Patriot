/**
RearPanel Controller
Description: This sketch controls all the switches in the Cyclone 4005 rear control panel.
Author: Ron Lisle

 The controller is located in the office above the piano.
 Power provided via Fuse #13, #14
 
 Updated to Photon 2
 - No Photon 2 pins used directly.
 - Use I2C for all I/O.
 
 To update code:
   1. Edit this code
   2. Update IoT if needed
   3. Put Photon into safe mode using buttons (breathing magenta)
        Press both buttons, release reset, release setup when blinking magenta
   4. "frp2" or "particle flash rear_panel2"
 
 Since Cloud is not connected, photon shuld normally be breathing green

 Using SYSTEM_THREAD(ENABLED) is recommended,
 and runs network on separate theread.
 Using SYSTEM_MODE(AUTOMATIC)

 Migration from Photon 1 to Photon 2:
 1. Remove 12v power and disconnect current I2CR4IO4 board - note/photo wires
 2. Remove SCT board - note wires
 3. Install IoTR4IO4 with Photon 2 and adapter and USB power converter
 4. Reconnect wires to IoTR4IO4 board
 5. Reprogram/configure Address
 6. Reprogram FIR to use R4IO4
 
 Compiling: particle compile photon2 --target 5.4.1
 Flashing: particle flash rear_panel2 --target 5.4.1
   or shortcut "frp2"
 */

// !!!!
// NOTE: Deprecated - refer to RearPanel2.cpp
// !!!!

#include <IoT.h>
#include "secrets.h"   // Modify this to include your passwords: HUE_USERID
#include "math.h"

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(AUTOMATIC);

#define CONTROLLER_NAME "RearPanel"
#define MQTT_BROKER "192.168.0.33"
#define OFFICE_MOTION_TIMEOUT 15
#define PCA9634_ADDRESS 1       // 1st jumper
#define I2CR4IO4_ADDRESS 0x20  // 4xRelay+4GPIO address (0x20 = no jumpers)

// Generally uncomment only 1 of the following 2 lines
#define MQTT_LOGGING true
//SerialLogHandler logHandler1(57600, LOG_LEVEL_ALL);

bool officeMotion = false;
long lastOfficeMotion = 0;
bool officeDoor = false;
bool officeDoorCountdown = false;
long lastOfficeDoor = 0;

void setup() {
//    WiFi.setCredentials("LalaIoT", "01120112");
    WiFi.selectAntenna(ANT_INTERNAL);
    
    //WiFi.useDynamicIP();
    IoT::begin(MQTT_BROKER, CONTROLLER_NAME, MQTT_LOGGING);
    
    //Consolidate PCA9634 initialization
    MCP23008::initialize(I2CR4IO4_ADDRESS, 0xf0);   // Address 0x20 (no jumpers), all 4 GPIOs inputs
    PCA9634::initialize(PCA9634_ADDRESS);
    createDevices();
}


void createDevices() {
    // I2CIO4R4G5LE board
    // 4 Relays
    Device::add(new Curtain(0, "Curtain", "Office"));     // 2x Relays: 0, 1
    // Device::add(new Awning(2, "RearAwning", "Outside")); // 2x Relays: 2, 3
    
    // 4 GPIO
    Device::add(new NCD4Switch(1, "OfficeDoor", "Office"));
//    Device::add(new NCD4PIR(I2CR4IO4_ADDRESS, 1, "OfficeMotion", "Office", OFFICE_MOTION_TIMEOUT));

    // (deprecated) Photon I/O
    //Device::add(new PIR(A5, "OfficeMotion", "Office", OFFICE_MOTION_TIMEOUT));

    // I2CPWM8W80C board
    // 8 Dimmers
    Device::add(new NCD8Light(1, "OfficeCeiling", "Office"));
    Device::add(new NCD8Light(2, "Loft", "Office"));
    Device::add(new NCD8Light(3, "RampPorch", "Outside"));
    Device::add(new NCD8Light(4, "RampAwning", "Outside"));
    Device::add(new NCD8Light(5, "RearPorch", "Outside"));
    Device::add(new NCD8Light(6, "RearAwning", "Outside"));
    Device::add(new NCD8Light(7, "Piano", "Office"));
    //Device::add(new NCD8Light(8, "Unused", "Office"));

    // Pseudo Devices
    Device::add(new Device("AnyoneHome", "All", 'X'));
    Device::add(new Device("RonHome", "All", 'X'));
    Device::add(new Device("ShelleyHome", "All", 'X'));
    Device::add(new Device("Nighttime", "All", 'X'));

    // Checklist Items -  - define for every non-automated checklist item
    
    // Pre-Trip checklist items
    Device::add(new Device("startList", "All", 'X'));
    Device::add(new Device("checkTires", "All", 'X'));
    Device::add(new Device("dumpTanks", "All", 'X'));
    Device::add(new Device("fillWater", "All", 'X'));
    Device::add(new Device("fuel", "All", 'X'));
    Device::add(new Device("fillPropane", "All", 'X'));
    Device::add(new Device("checkRoof", "All", 'X'));
    Device::add(new Device("checkUnderRV", "All", 'X'));
    Device::add(new Device("planRoute", "All", 'X'));

    // Depart checklist items
    Device::add(new Device("bedSlideIn", "All", 'X'));
    Device::add(new Device("LRSlideIn", "All", 'X'));
    Device::add(new Device("rampAwningIn", "All", 'X'));
    Device::add(new Device("closeRamp", "All", 'X'));
    Device::add(new Device("rearAwningIn", "All", 'X'));
    Device::add(new Device("latchHandles", "All", 'X'));
    Device::add(new Device("frontAwningIn", "All", 'X'));
    Device::add(new Device("discPropane", "All", 'X'));
    Device::add(new Device("waterHeaterOff", "All", 'X'));
    Device::add(new Device("hitchTruck", "All", 'X'));
    Device::add(new Device("raiseLG", "All", 'X'));
    Device::add(new Device("discPower", "All", 'X'));

    // Arrive checklist items
    Device::add(new Device("connectPower", "All", 'X'));
    Device::add(new Device("lowerLG", "All", 'X'));
    Device::add(new Device("bedSlideOut", "All", 'X'));
    Device::add(new Device("LRSlidesOut", "All", 'X'));
    Device::add(new Device("openRamp", "All", 'X'));
    Device::add(new Device("rampAwningOut", "All", 'X'));
    Device::add(new Device("stepsDown", "All", 'X'));
    Device::add(new Device("rearAwningOut", "All", 'X'));
    Device::add(new Device("frontAwningOut", "All", 'X'));
    Device::add(new Device("propaneOn", "All", 'X'));
    Device::add(new Device("waterHose", "All", 'X'));
    Device::add(new Device("waterHeaterOn", "All", 'X'));
    Device::add(new Device("sewerHose", "All", 'X'));
}
void loop() {
    IoT::loop();
}
