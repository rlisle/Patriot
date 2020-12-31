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

#define ADDRESS 1   // PWM board address A0 jumper set

String mqttServer = "192.168.10.184";

IoT *iot;


void setup() {
    
    iot = IoT::getInstance();
    iot->setControllerName("RearPanel");
    iot->begin();
    iot->connectMQTT(mqttServer, "PatriotRearPanel1", false);

    // Lights
    iot->addDevice(new NCD8Light(ADDRESS, 0, "OfficeCeiling", 2));
    iot->addDevice(new NCD8Light(ADDRESS, 1, "Loft", 2));
    iot->addDevice(new NCD8Light(ADDRESS, 2, "RampPorch", 2));
    iot->addDevice(new NCD8Light(ADDRESS, 3, "RampAwning", 2));
    iot->addDevice(new NCD8Light(ADDRESS, 4, "RearPorch", 2));
    iot->addDevice(new NCD8Light(ADDRESS, 5, "RearAwning", 2));
    iot->addDevice(new NCD8Light(ADDRESS, 6, "Piano", 2));
    // one unused dimmer I/O

    // Switches
    // Switches provide backup control for when Alexa is not available.
    // This can happen when the internet is not available.
    iot->addDevice(new Switch(A0, "OfficeCeilingSwitch"));
    iot->addDevice(new Switch(A1, "LoftSwitch"));
    iot->addDevice(new Switch(A2, "RampPorchSwitch"));
    iot->addDevice(new Switch(A3, "RampAwningSwitch"));
    iot->addDevice(new Switch(A4, "RearPorchSwitch"));
    iot->addDevice(new Switch(A5, "RearAwningSwitch"));
    // More available inputs A6, A7, TX, RX - use for door switch, motion detector, etc.

    // Note: Activities and PartOfDay are defined in RonTest instead of here
}

void loop() {

    // When IoT loop() is call, it will
    // - set all previous levels
    // - read switches and update levels
    // - update light dimming
    iot->loop();
    
//    State* sleeping = iot->getState("sleeping");
//    State* partOfDay = iot->getState("partofday");
//
//    if( sleeping != NULL && sleeping->hasChanged() ) {
//
//        Log.info("sleeping has changed");
//
//        // Alexa, Good morning
//        if( sleeping->value() == AWAKE && partOfDay->value() > SUNSET ) {
//            iot->setDeviceValue("OfficeCeiling", 30);
//        }
//
//        // Alexa, Bedtime
//        if( sleeping->value() == RETIRING ) {
//            iot->publishValue("cleaning", 0);
//            iot->publishValue("cooking", 0);
//
//            iot->setDeviceValue("OfficeCeiling", 30);
//            iot->setDeviceValue("Loft", 0);
//            iot->setDeviceValue("RampPorch", 0);
//            iot->setDeviceValue("RampAwning", 0);
//            iot->setDeviceValue("RearProch", 0);
//            iot->setDeviceValue("RearAwning", 0);
//            iot->setDeviceValue("Piano", 0);
//        }
//
//        // Alexa, Goodnight
//        if( sleeping->value() == ASLEEP ) {
//            iot->publishValue("cleaning", 0);
//            iot->publishValue("cooking", 0);
//
//            iot->setDeviceValue("OfficeCeiling", 0);
//            iot->setDeviceValue("Loft", 0);
//            iot->setDeviceValue("RampPorch", 0);
//            iot->setDeviceValue("RampAwning", 0);
//            iot->setDeviceValue("RearProch", 0);
//            iot->setDeviceValue("RearAwning", 0);
//            iot->setDeviceValue("Piano", 0);
//        }
//    }
//
//    if( partOfDay != NULL && partOfDay->hasChanged() ) {
//
//        Log.info("partOfDay has changed");
//
//        if( partOfDay->value() == SUNRISE ) {
//            // Turn off lights at sunrise
//            iot->setDeviceValue("OfficeCeiling", 0);
//            iot->setDeviceValue("Loft", 0);
//            iot->setDeviceValue("RampPorch", 0);
//            iot->setDeviceValue("RampAwning", 0);
//            iot->setDeviceValue("RearProch", 0);
//            iot->setDeviceValue("RearAwning", 0);
//            iot->setDeviceValue("Piano", 0);
//        }
//
//        if( partOfDay->value() == DUSK ) {
//            // Turn on lights after sunset
//            iot->setDeviceValue("OfficeCeiling", 50);
//            iot->setDeviceValue("Loft", 0);
//            iot->setDeviceValue("RampPorch", 50);
//            iot->setDeviceValue("RampAwning", 100);
//            iot->setDeviceValue("RearProch", 60);
//            iot->setDeviceValue("RearAwning", 100);
//            iot->setDeviceValue("Piano", 30);
//        }
//    }
//
//    if( iot->didTurnOn("cleaning") ) {
//        Log.info("cleaning did turn on");
//        setAllInsideLights( 100 );
//    } else if( iot->didTurnOff("cleaning") ) {
//        Log.info("cleaning did turn off");
//        setAllInsideLights( 0 );
//    }

    // SWITCHES
    iot->handleLightSwitch("OfficeCeiling");
    iot->handleLightSwitch("Loft");
    iot->handleLightSwitch("RampPorch");
    iot->handleLightSwitch("RampAwning");
    iot->handleLightSwitch("RearPorch");
    iot->handleLightSwitch("RearAwning");
}

void setAllInsideLights(int level) {
    Log.info("setAllInsideLights %d",level);
    iot->setDeviceValue("OfficeCeiling", level);
    iot->setDeviceValue("Loft", level);
    iot->setDeviceValue("Piano", level);
}

void setAllOutsideLights(int level) {
    Log.info("setAllOutsideLights %d",level);
    iot->setDeviceValue("RampPorch", level);
    iot->setDeviceValue("RampAwning", level);
    iot->setDeviceValue("RearProch", level);
    iot->setDeviceValue("RearAwning", level);

}
