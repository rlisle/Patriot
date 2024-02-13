/* 
 * Project Patriot RearPanel
 * Author: Ron Lisle
 * Date: 1/5/24
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 * 
 * To update code:
 *  1. Edit this code (VSCode or Xcode)
 *  2. Update IoT if needed
 * If USB connected using VSCode:
 *  3. Put Photon into safe mode using buttons (breathing magenta):
 *     Press both buttons, release reset, release setup when blinking magenta
 *  4. "frp2" or "particle flash rear_panel2"
 * If OTA
 *  3. "frp"
 * 
 * Compiling: particle compile photon2 --target 5.6.0
 * Flashing: particle flash rear_panel2 --target 5.6.0 or shortcut "frp"
 * 
 * PIR connected via I2CIO4R4 board
 *   GPIO connector: GP4, Gnd, GP5, GP6, Gnd, GP7
 *   GP4 = Door, GP5 = PIR Power, GP6 = PIR Input, GP7 n/c
 *   
 */

#include <IoT.h>

// Generally uncomment only 1 of the following 2 logs
#define MQTT_LOGGING true
// View logs with CLI using 'particle serial monitor --follow'
//SerialLogHandler logHandler1(57600, LOG_LEVEL_INFO);

//TODO: change to const
#define CONTROLLER_NAME "RearPanel"
#define MQTT_BROKER "192.168.0.33"

//TODO: move timeouts into IoT
#define OFFICE_MOTION_TIMEOUT_MSECS 60*1000
#define OFFICE_DOOR_TIMEOUT_MSECS 7*60*1000

#define PCA9634_ADDRESS 1       // 1st jumper
#define I2CR4IO4_ADDRESS 0x20  // 4xRelay+4GPIO address (0x20 = no jumpers)

//TODO: move into IoT
typedef unsigned long msecs;

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(AUTOMATIC);

// State
bool ronIsHome = true;
bool shelleyIsHome = true;
bool anyoneIsHome = true;
bool nighttime = true;
bool sleeping = true;
bool officeMotion = false;
bool officeDoorOpen = false;
bool cleaning = false;

// Timing
//TODO: move into IoT
msecs lastMinute = 0;

bool isTimingOfficeMotion;
msecs msecsLastOfficeMotion = 0;

bool isTimingOfficeDoor = false;
msecs msecsLastDoorEvent = 0;

// Behaviors
#include "Behaviors.h"
#include "EventHandlers.h"

//-------------
// LOOP
//-------------
void loop() {
    IoT::loop();

    if(msecs()-60*1000 > lastMinute) {
        lastMinute = msecs();
        handleNextMinute();
    }
}

void setup() {
//    WiFi.setCredentials(WIFI_SSID, WIFI_PASSWORD);
//    WiFi.selectAntenna(ANT_INTERNAL);    
//    WiFi.useDynamicIP();

    IoT::begin(MQTT_BROKER, CONTROLLER_NAME, MQTT_LOGGING);
    
    //Consolidate PCA9634 initialization
    // GPIO4 = Door, GPIO5 = PIR Power, GPIO6 = PIR Input, GPIO7 n/c
    MCP23008::initialize(I2CR4IO4_ADDRESS, 0xd0);   // Address 0x20 (no jumpers)
    PCA9634::initialize(PCA9634_ADDRESS);

    MCP23008::write(5,true);   // Apply power to PIR. Pin can source 25ma

    // Behaviors
    Device::add(new Device("AnyoneHome", "Status", 'S', handleAnyoneHome));
    Device::add(new Device("RonHome", "Status", 'S', handleRonHome));
    Device::add(new Device("ShelleyHome", "Status", 'S', handleShelleyHome));
    Device::add(new Device("Nighttime", "Status", 'S', handleNighttime));
    Device::add(new Device("Sleeping", "Status", 'S', handleSleeping));
    Device::add(new Device("Cleaning", "Status", 'S', handleCleaning));

    // I2CIO4R4G5LE board
    // 4 Relays
    Device::add(new Curtain(0, "Curtain", "Office"));     // 2x Relays: 0, 1
    // Device::add(new Awning(2, "RearAwning", "Outside")); // 2x Relays: 2, 3
    
    // 4 GPIO
    Device::add(new NCD4Switch(1, "OfficeDoor", "Office", handleOfficeDoor));
    Device::add(new NCD4PIR(3, "OfficeMotion", "Office", OFFICE_MOTION_TIMEOUT_MSECS, handleOfficeMotion));

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
