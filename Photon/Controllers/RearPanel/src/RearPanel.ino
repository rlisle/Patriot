/**
RearPanel Controller
Description: This sketch controls all the switches in the Cyclone 4005 rear control panel.
Author: Ron Lisle
 
 This is the bridge controller, so all activities should be added here.
 This controller will then repeat them to MQTT.
 
  To update Photon:
    1. Edit this code
    2. Update IoT and plugins if needed
    3. "particle flash RearPanel"
 
 */
#include <IoT.h>
#include <PatriotSwitch.h>
#include <PatriotNCD8Light.h>
#include <PatriotActivity.h>
#include <PatriotPartOfDay.h>

#define ADDRESS 1   // PWM board address A0 jumper set

String mqttServer = "192.168.10.184";

IoT *iot;


void setup() {
    
    iot = IoT::getInstance();
    iot->setControllerName("RearPanel");
    iot->begin();
    iot->connectMQTT(mqttServer, "PatriotRearPanel1", true);   // MQTT bridge enabled

    // Lights
    NCD8Light *ceiling = new NCD8Light(ADDRESS, 0, "OfficeCeiling", 2);
    NCD8Light *loft = new NCD8Light(ADDRESS, 1, "Loft", 2);
    NCD8Light *rampPorch = new NCD8Light(ADDRESS, 2, "RampPorch", 2);
    NCD8Light *rampAwning = new NCD8Light(ADDRESS, 3, "RampAwning", 2);
    NCD8Light *rearPorch = new NCD8Light(ADDRESS, 4, "RearPorch", 2);
    NCD8Light *rearAwning =new  NCD8Light(ADDRESS, 5, "RearAwning", 2);
    NCD8Light *piano = new NCD8Light(ADDRESS, 6, "Piano", 2);
    // one unused dimmer I/O

    // Switches
    Switch *officeSwitch = new Switch(A0, "OfficeSwitch");
    Switch *loftSwitch = new Switch(A1, "LoftSwitch");
    Switch *wakingSwitch = new Switch(A2, "WakingSwitch");
    Switch *awningSwitch = new Switch(A3, "AwningSwitch");
    Switch *floodsSwitch = new Switch(A4, "FloodsSwitch");
    Switch *pianoSwitch = new Switch(A5, "PianoSwitch");
    // More available inputs A6, A7, TX, RX - use for door switch, motion detector, etc.

    // Activities allow Alexa to control them directly or via routines
    // and can also turn off other activities.
    // These can be used by other panels also, but don't need to be duplicated by them
    Activity *cleaning = new Activity("cleaning");   // Turn on all main lights
    Activity *cooking = new Activity("cooking");     // Turn on lots of kitchen lights
    Activity *sleeping = new Activity("sleeping");   // 0=awake (good morning), 1=retiring (bedtime), 2=sleeping (good night)
    
    PartOfDay* partOfDay = new PartOfDay();

    // Switches
//    ceiling->addBehavior(100, "OfficeSwitch", '>', 0);
//    loft->addBehavior(100, "LoftSwitch", '>', 0);
//    rampAwning->addBehavior(100, "AwningSwitch", '>', 0);
//    rearAwning->addBehavior(100, "AwningSwitch", '>', 0);
//    rampPorch->addBehavior(100, "FloodSwitch", '>', 0);
//    rearPorch->addBehavior(100, "FloodSwitch", '>', 0);
//    piano->addBehavior(100, "pianoSwitch", '>', 0);

    // ADD ALL DEVICES
    iot->addDevice(ceiling);
    iot->addDevice(loft);
    iot->addDevice(rampPorch);
    iot->addDevice(rampAwning);
    iot->addDevice(rearPorch);
    iot->addDevice(rearAwning);
    iot->addDevice(piano);

    // ADD SWITCHES
    iot->addDevice(officeSwitch);
    iot->addDevice(loftSwitch);
    iot->addDevice(wakingSwitch);
    iot->addDevice(awningSwitch);
    iot->addDevice(floodsSwitch);
    iot->addDevice(pianoSwitch);
    
    // ADD ACTIVITIES
    iot->addDevice(cleaning);
    iot->addDevice(cooking);
    iot->addDevice(sleeping);
    
    // ADD OTHER
    iot->addDevice(partOfDay);
}

// Save previous states we care about
int prevSleeping = ASLEEP;
int prevPartOfDay = NIGHT;
int prevCleaning = 0;

// Since everything happens in loop(), we shouldn't need
// to worry about states changing asynchronously while
// we are processing them
// TODO: refactor previous/didChange into IoT
void loop() {
    int sleeping = iot->getState("sleeping");
    int partOfDay = iot->getState("partofday");
    
    // Sleeping turns off other states
    if( sleeping != prevSleeping ) {
        
        // Alexa, Good morning
        if( sleeping == AWAKE && partOfDay > SUNSET ) {
            iot->setDevice("OfficeCeiling", 30);
        }
        
        // Alexa, Bedtime
        if( sleeping == RETIRING ) {
            iot->publishState("cleaning", 0);
            iot->publishState("cooking", 0);
            
            iot->setDevice("OfficeCeiling", 30);
            iot->setDevice("Loft", 0);
            iot->setDevice("RampPorch", 0);
            iot->setDevice("RampAwning", 0);
            iot->setDevice("RearProch", 0);
            iot->setDevice("RearAwning", 0);
            iot->setDevice("Piano", 0);
        }
        
        // Alexa, Goodnight
        if( sleeping == ASLEEP ) {
            iot->publishState("cleaning", 0);
            iot->publishState("cooking", 0);
            
            iot->setDevice("OfficeCeiling", 0);
            iot->setDevice("Loft", 0);
            iot->setDevice("RampPorch", 0);
            iot->setDevice("RampAwning", 0);
            iot->setDevice("RearProch", 0);
            iot->setDevice("RearAwning", 0);
            iot->setDevice("Piano", 0);
        }
        
        prevSleeping = sleeping; // Refactor to IoT
    }
    
    if( partOfDay != prevPartOfDay ) {
        
        if( partOfDay == SUNRISE ) {
            // Turn off lights at sunrise
            iot->setDevice("OfficeCeiling", 0);
            iot->setDevice("Loft", 0);
            iot->setDevice("RampPorch", 0);
            iot->setDevice("RampAwning", 0);
            iot->setDevice("RearProch", 0);
            iot->setDevice("RearAwning", 0);
            iot->setDevice("Piano", 0);
        }
        
        if( partOfDay == DUSK ) {
            // Turn on lights after sunset
            iot->setDevice("OfficeCeiling", 50);
            iot->setDevice("Loft", 0);
            iot->setDevice("RampPorch", 50);
            iot->setDevice("RampAwning", 100);
            iot->setDevice("RearProch", 50);
            iot->setDevice("RearAwning", 100);
            iot->setDevice("Piano", 30);
        }
        
        prevPartOfDay = partOfDay;
    }
    
    if( cleaning != prevCleaning ) {
        int cleaningLevel = (cleaning==0) ? 0 : 100;
        
        iot->setDevice("OfficeCeiling", cleaningLevel);
        iot->setDevice("Loft", cleaningLevel);
        iot->setDevice("Piano", cleaningLevel);

        prevCleaning = cleaning;
    }
    
    iot->loop();
}
