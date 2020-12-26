/**
  FrontPanel Controller
  Description: This sketch controls all the switches in the Cyclone 4005 front control panel.
  Author: Ron Lisle
  Date: 9/16/17
 
 This isn't the bridge controller, so we shouldn't receive any activities from particle.io
 
  To update Photon:
    1. Edit this code
    2. Update IoT and plugins if needed
    2. "particle flash FrontPanel"
 
 */
#include <IoT.h>
#include <PatriotLight.h>
#include <PatriotSwitch.h>
#include <PatriotNCD8Light.h>

#define ADDRESS2 1      // PWM board switches low switch on

String mqttServer = "192.168.10.184";

IoT *iot;

              
void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("FrontPanel");
    iot->begin();
    iot->connectMQTT(mqttServer, "patriotFrontPanel1");

    iot->addDevice(new NCD8Light(ADDRESS2, 0, "DoorSide", 2));
    iot->addDevice(new NCD8Light(ADDRESS2, 1, "KitchenCeiling", 2));
    iot->addDevice(new NCD8Light(ADDRESS2, 2, "Sink", 2));
    iot->addDevice(new NCD8Light(ADDRESS2, 3, "OtherSide", 2));
    iot->addDevice(new NCD8Light(ADDRESS2, 4, "RightTrim",2));
    iot->addDevice(new NCD8Light(ADDRESS2, 5, "LeftTrim",2));
    iot->addDevice(new NCD8Light(ADDRESS2, 6, "FrontAwning", 2));
    iot->addDevice(new NCD8Light(ADDRESS2, 7, "FrontPorch", 2));

    iot->addDevice(new Light(D2, "Ceiling", 2));
    iot->addDevice(new Light(D3, "Cabinets", 2));

    // Switch wiring
    // Top left: RX     tape "Cabinet"
    // Top right: TX
    // 2nd left: A7     tape "Sink"
    // 2nd middle: A5   tape "Ceiling"
    // 2nd right: A6    tape "Indirect"
    // 3rd left: A1     "Ceiling"
    // 3rd l-m: A2      "DS Flood Lights"
    // 3rd r-m: A3      "ODS Flood Lights"
    // 3rd right: A4    "Porch Lights"
    // bottom: A0       "Light"
    iot->addDevice(new Switch(A5, "CabinetsSwitch"));        // tape "Cabinet"     (top left)
    iot->addDevice(new Switch(A6, "RightTrimSwitch"));      //                    (top right)
    iot->addDevice(new Switch(A7, "SinkSwitch"));           // tape "Sink"        (2nd left)
    iot->addDevice(new Switch(RX, "KitchenCeilingSwitch")); // tape "Ceiling"  (2nd middle)
    iot->addDevice(new Switch(TX, "LeftTrimSwitch"));       // tape "Indirect"    (2nd right)
    
    iot->addDevice(new Switch(A1, "CeilingSwitch"));        // "Ceiling"          (3rd left)
    iot->addDevice(new Switch(A2, "DoorSideSwitch"));       // "DS Flood Lights"  (3rd l-m)
    iot->addDevice(new Switch(A3, "OtherSideSwitch"));      // "ODS Flood Lights" (3rd m-r)
    iot->addDevice(new Switch(A4, "FrontPorchSwitch"));     // "Porch Lights"     (3rd right)
    iot->addDevice(new Switch(A0, "FrontAwningSwitch"));    // "Light"            (bottom)
}

void loop() {
    
    // When IoT loop() is call, it will
    // - set all previous levels
    // - read switches and update levels
    // - update light dimming
    iot->loop();
    
    State* sleeping = iot->getState("sleeping");
    State* partOfDay = iot->getState("partofday");

    if( sleeping != NULL && sleeping->hasChanged() ) {

        Log.info("sleeping has changed: %d",sleeping->value());
        
        // Alexa, Good morning
        if( sleeping->value() == AWAKE && partOfDay->value() > SUNSET ) {
            setMorningLights();
        }
        
        // Alexa, Bedtime
        if( sleeping->value() == RETIRING ) {
            setMorningLights();
            setAllOutsideLights(0);
        }
        
        // Alexa, Goodnight
        if( sleeping->value() == ASLEEP ) {
            setAllOutsideLights(0);
            setAllInsideLights(0);
        }
    }
    
    if( partOfDay != NULL && partOfDay->hasChanged() ) {
        
        Log.info("PartOfDay has changed: %d", partOfDay->value());
        
        if( partOfDay->value() == SUNRISE ) {
            setAllOutsideLights(0);
            setAllInsideLights(0);
        }
        
        if( partOfDay->value() == DUSK ) {
            setEveningLights();
        }
    }
    
    iot->handleLightSwitch("Ceiling");
    iot->handleLightSwitch("KitchenCeiling");
    iot->handleLightSwitch("Sink");
    iot->handleLightSwitch("Cabinets");
    iot->handleLightSwitch("RightTrim");
    iot->handleLightSwitch("LeftTrim");
    iot->handleLightSwitch("DoorSide");
    iot->handleLightSwitch("OtherSide");
    iot->handleLightSwitch("FrontPorch");
    iot->handleLightSwitch("FrontAwning");
}

void setMorningLights() {
    iot->setDeviceValue("KitchenCeiling", 50);
    iot->setDeviceValue("Sink", 50);
    iot->setDeviceValue("RightTrim", 0);
    iot->setDeviceValue("LeftTrim", 0);
    iot->setDeviceValue("Ceiling", 0);
    iot->setDeviceValue("Cabinets", 50);
}

void setEveningLights() {
    iot->setDeviceValue("KitchenCeiling", 50);
    iot->setDeviceValue("Sink", 50);
    iot->setDeviceValue("RightTrim", 0);
    iot->setDeviceValue("LeftTrim", 100);
    iot->setDeviceValue("Ceiling", 50);
    iot->setDeviceValue("Cabinets", 50);
    
    setAllOutsideLights(100);
}

void setAllInsideLights(int level) {
    iot->setDeviceValue("KitchenCeiling", level);
    iot->setDeviceValue("Sink", level);
    iot->setDeviceValue("RightTrim", level);
    iot->setDeviceValue("LeftTrim", level);
    iot->setDeviceValue("Ceiling", level);
    iot->setDeviceValue("Cabinets", level);
}

void setAllOutsideLights(int level) {
    iot->setDeviceValue("DoorSide", level);
    iot->setDeviceValue("OtherSide", level);
    iot->setDeviceValue("FrontAwning", level);
    iot->setDeviceValue("FrontPorch", level);
}
