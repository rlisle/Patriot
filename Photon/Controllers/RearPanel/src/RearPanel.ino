/**
RearPanel Controller
Description: This sketch controls all the switches in the Cyclone 4005 rear control panel.
Author: Ron Lisle

 The controller is located in the office above the piano.
 It includes the PartOfDay plugin.
 
 Photon pin assignments
 - A2-A4 future 12v monitor ?
 - A5 PIR
 
 To update Photon:
   1. Edit this code
   2. Update IoT and plugins if needed
   3. Put Photon into safe mode using buttons (breathing magenta)
        Press both buttons, release reset, release setup when blinking magenta
   4. "particle flash RearPanel" or "frp"
 
 Since Cloud is not connected, photon should normally be breathing green

 Using SYSTEM_THREAD(ENABLED) is recommended,
 and runs network on separate theread.
 Using SYSTEM_MODE(SEMI_AUTOMATIC) we will
 manually connect, but everything is automatic
 after that. This allows running loop and MQTT
 even if no internet available

 */
#include <IoT.h>
#include <PatriotNCD8Light.h>
#include <PatriotCurtain.h>
#include <PatriotNCD4Switch.h>
#include <PatriotNCD4Relay.h>
#include <PatriotPIR.h>
//#include "secrets.h"   // Modify this to include your passwords: HUE_USERID

#define CONTROLLER_NAME "RearPanel"
#define MQTT_BROKER "192.168.50.33"
#define OFFICE_MOTION_TIMEOUT 2*60*1000
#define OFFICE_DOOR_TIMEOUT 5*60*1000

#define ADDRESS 1      // PWM board address A0 jumper set
#define I2CR4IO4 0x20  // 4xRelay+4GPIO address

#define CONNECT_TO_CLOUD true
//SYSTEM_THREAD(ENABLED);
//SYSTEM_MODE(SEMI_AUTOMATIC);

bool officeMotion = false;
long lastOfficeMotion = 0;

bool officeDoor = false;
bool officeDoorCountdown = false;
long lastOfficeDoor = 0;

void setup() {
    WiFi.selectAntenna(ANT_EXTERNAL);
    WiFi.useDynamicIP();
    IoT::begin(MQTT_BROKER, CONTROLLER_NAME, CONNECT_TO_CLOUD);
    createDevices();
}

void createDevices() {
    // I2CIO4R4G5LE board
    // 4 Relays
    Device::add(new Curtain(I2CR4IO4, 0, "Curtain", "Office"));     // 2x Relays: 0, 1
    // Fading OfficeTrim results in door toggling, probably due to parallel wiring, so on/off only
    Device::add(new NCD4Relay(I2CR4IO4, 2, "OfficeRightTrim", "Office"));
    
    // 4 GPIO
    Device::add(new NCD4Switch(I2CR4IO4, 0, "OfficeDoor", "Office"));
    
    // Photon I/O
    Device::add(new PIR(A5, "OfficeMotion", "Office", OFFICE_MOTION_TIMEOUT));

    // I2CPWM8W80C board
    // 8 Dimmers
    Device::add(new NCD8Light(ADDRESS, 0, "OfficeCeiling", "Office", 2));
    Device::add(new NCD8Light(ADDRESS, 1, "Loft", "Office", 2));
    Device::add(new NCD8Light(ADDRESS, 2, "RampPorch", "Outside", 2));
    Device::add(new NCD8Light(ADDRESS, 3, "RampAwning", "Outside", 2));
    Device::add(new NCD8Light(ADDRESS, 4, "RearPorch", "Outside", 2));
    Device::add(new NCD8Light(ADDRESS, 5, "RearAwning", "Outside", 2));
    Device::add(new NCD8Light(ADDRESS, 6, "Piano", "Office", 2));
    Device::add(new NCD8Light(ADDRESS, 7, "OfficeLeftTrim", "Office", 2));
    
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

/**
 * LOOP
 */
void loop() {
    IoT::loop();
    
//    handleOfficeMotion();
//    handleOfficeDoor();
}

/**
 * handleOfficeMotion
 *
 * Dependencies:
 *   int sleeping
 *   int partOfDay
 */
//void handleOfficeMotion() {
//
//    long loopTime = millis();
//    int officeMotionChanged = Device::getChangedValue("OfficeMotion");
//
//    if(officeMotionChanged == 100) {
//        Log.info("Office Motion detected");
//        Device::setValue("OfficeCeiling", 20);
//        officeMotion = true;
//
//        // Determine if this is Ron getting up
//        if( partOfDay > SUNSET && sleeping != AWAKE) {
//            //TODO: maybe blink instead?
//            Device::setValue("OfficeCeiling", 40);
//            if(Time.hour() > 3 && Time.hour() < 9) {   // Motion after 4:00 is wakeup
//                Device::setValue("OfficeCeiling", 60);
//                IoT::mqttPublish("patriot/sleeping", "1");   // AWAKE
//                Device::setValue("sleeping", AWAKE);
//            }
//        }
//
//    //TODO: Use a timer to turn off motion activated lights (like office door)
//    } else if(officeMotionChanged == 0) {
//        Device::setValue("OfficeCeiling", 0);
//        officeMotion = false;
//
//    } // Ignore -1
//}

/**
 * handleOfficeDoor
 *
 * Dependencies:
 *   int sleeping
 *   int partOfDay
 */
//void handleOfficeDoor() {
//
//    // Timed shut-off after door closes
//    long loopTime = millis();
//    if(officeDoorCountdown == true) {
//        if(loopTime >= lastOfficeDoor+OFFICE_DOOR_TIMEOUT) {
//            Log.info("Office door timeout");
//            officeDoorCountdown = false;
//            Device::setValue("RearPorch", 0);
//        }
//    }
//
//    int officeDoorChanged = Device::getChangedValue("OfficeDoor");
//    if( officeDoorChanged != -1) {
//        if( officeDoorChanged > 0 ) {   // Door opened
//            officeDoor = true;
//            officeDoorCountdown = false;    // Reset it if it was in progress
//            // If after sunset turn on porch light
//            if( partOfDay > SUNSET ) {
//                Device::setValue("RearPorch", 100);
//            }
//        } else {                        // Door closed
//            officeDoor = false;
//            lastOfficeDoor = millis();  // update timeout
//            officeDoorCountdown = true;
//            // Nothing else to do when door closes. Timer will shut off if needed.
//        }
//    }
//
//}

//void setAllInsideLights(int value) {
//    Log.info("setAllInsideLights %d",value);
//    Device::setValue("OfficeCeiling", value);
//    Device::setValue("Loft", value);
//    Device::setValue("Piano", value);
//    Device::setValue("OfficeTrim", value);
//
//}
//
//void setAllOutsideLights(int value) {
//    Log.info("setAllOutsideLights %d",value);
//    Device::setValue("RampPorch", value);
//    Device::setValue("RampAwning", value);
//    Device::setValue("RearPorch", value);
//    Device::setValue("RearAwning", value);
//}
