/*
 * RonTest Controller
 *
 * Description: This sketch provides the bridge, PartOfDay, and Activities
 *
 * Author: Ron Lisle
 *
 * Hardware
 * - built-in blue LED     D7
 */

/// BEGINNING OF MQTT THREADED EXAMPLE
#include "MQTT.h"

SYSTEM_THREAD(ENABLED);
void callback(char* topic, byte* payload, unsigned int length);

SerialLogHandler logHandler;

/**
 * if want to use IP address,
 * const uint8_t[] = { XXX,XXX,XXX,XXX };
 * MQTT client(server, 1883, callback);
 * want to use domain name,
 * exp) iot.eclipse.org is Eclipse Open MQTT Broker: https://iot.eclipse.org/getting-started
 * MQTT client("mqtt.eclipse.org", 1883, callback, true);
 *      4th parameter : bool thread(default false.)
 *      SYSTEM_THREAD(ENABLED) settings : thread is true.
 **/
MQTT client("sample.com", 1883, callback, true);

// recieve message
void callback(char* topic, byte* payload, unsigned int length) {
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = NULL;

    Log.info("MQTT received: "+String(p));
    if (!strcmp(p, "RED"))
        RGB.color(255, 0, 0);
    else if (!strcmp(p, "GREEN"))
        RGB.color(0, 255, 0);
    else if (!strcmp(p, "BLUE"))
        RGB.color(0, 0, 255);
    else
        RGB.color(255, 255, 255);
    delay(1000);
}


void setup() {
    WiFi.on();
    WiFi.setCredentials("ssid", "pass");
    WiFi.connect();
    waitUntil(WiFi.ready);
    while (WiFi.localIP() == IPAddress()) {
        delay(10);
    }

    // set LED control
    RGB.control(true);

    // connect to the server
    client.connect("sparkclient");

    // publish/subscribe
    if (client.isConnected()) {
        client.publish("outTopic/message","hello world");
        client.subscribe("inTopic/message");
    }
}

void loop() {
    if (client.isConnected())
      client.loop();
    delay(100);
}
/// END OF MQTT THREADED EXAMPLE







//#include "MQTT.h"
//#include <IoT.h>

// Running without cloud means Photon must be put into Listen mode to flash
//#define CLOUD_ENABLED false
//#define MQTT_LOGGING false

#define MQTT_TIMEOUT_SECONDS 60*16
#define MQTT_ALIVE_SECONDS 60*5
#define BLINK_INTERVAL  250

//SYSTEM_THREAD(ENABLED);
//SYSTEM_MODE(SEMI_AUTOMATIC);

//void callback(char* topic, byte* payload, unsigned int length);

//SerialLogHandler logHandler;

//unsigned long lastScan = 0;
//unsigned long scanInterval = 15000;

//int sleeping = 0;

//MQTT client("192.168.50.33", 1883, callback, true);

//long lastAliveTime = 0;
//int  status = 0;
//long lastBlinkTimeMs = 0;
//int blinkPhase = 0;
//long lastMQTTtime = 0;


//void setup() {
////    WiFi.selectAntenna(ANT_EXTERNAL); //TODO: connect external antenna
////    WiFi.useDynamicIP();
////      IoT::begin("192.168.50.33", "RonTest", CLOUD_ENABLED, MQTT_LOGGING);
////      Device::add(new Device("sleeping", "All"));
//
//    // Setup blue LED for network status
//    pinMode(D7, OUTPUT);    // Blue LED
//    digitalWrite(D7, LOW);
//
//    //Perform local setup
//    WiFi.on();
//    WiFi.connect();
//    waitUntil(WiFi.ready);
//    while (WiFi.localIP() == IPAddress()) {
//        delay(10);
//    }
//    client.connect("rontest01");
//    if (client.isConnected()) {
//        client.publish("test","RonTest setup");
//        client.subscribe("#");
//    }
//
//}

//void loop() {
////    IoT::loop();
////    handleSleeping();
////    scanI2Caddresses();
//
//    // Run local loop
//
//    if (client.isConnected())
//      client.loop();
//    delay(100);
//
//    updateStatusLed();
//    sendAlivePeriodically();
//}
//
//void sendAlivePeriodically() {
//    if(Time.now() > lastAliveTime + MQTT_ALIVE_SECONDS) {
//        lastAliveTime = Time.now();
//        String time = Time.format(Time.now(), "%a %H:%M");
//        client.publish("alive", time);
//    }
//}
//
//
//void callback(char* rawTopic, byte* payload, unsigned int length) {
//    char p[length + 1];
//    memcpy(p, payload, length);
//    p[length] = 0;
//    String message(p);
//    String topic(rawTopic);
//
//    lastMQTTtime = Time.now();
//
//    if (message == "1")
//        status = 1;
//    else if (message == "2")
//        status = 2;
//    else if (message == "3")
//        status = 3;
//    else
//        // Unknown - ignore it
//    delay(1000);
//}
//
//void doReboot() {
//    Log.warn("Resetting");
//    System.reset(RESET_NO_WAIT);
//}
//
//void updateStatusLed() {
//
//    if(millis() >= lastBlinkTimeMs + BLINK_INTERVAL) {
//
//        lastBlinkTimeMs = millis();
//        blinkPhase++;
//
//        int currentLed = digitalRead(D7);
//        int nextLed = LOW;
//
//        switch (status)
//        {
//            case 3:  // 3 short blinks off, on, off, on, off, off
//                if(blinkPhase == 1 || blinkPhase == 3 || blinkPhase == 5) {
//                    nextLed = HIGH;
//                } else if(blinkPhase > 7) {
//                    blinkPhase = 0;
//                }
//                break;
//            case 2: // 2 short blink off, on, off, off
//                if(blinkPhase == 1 || blinkPhase == 3) {
//                    nextLed = HIGH;
//                } else if(blinkPhase > 5) {
//                    blinkPhase = 0;
//                }
//                break;
//            case 1:    // Steady long blinks
//                nextLed = currentLed;
//                if(blinkPhase > 1) {
//                    nextLed = !currentLed;
//                    blinkPhase = 0;
//                }
//                break;
//            default:    // 0 = blue LED off
//                break;  // leave LOW
//        }
//        digitalWrite(D7, nextLed);
//    }
//}


//void handleSleeping() {
//
//    int sleepingChanged = Device::getChangedValue("sleeping");
//    if( sleepingChanged != -1 ) {
//
//        Log.info("sleeping has changed %d",sleepingChanged);
//
//        sleeping = sleepingChanged;
//    }
//}


// Diagnostic Functions
// Search all I2C addresses every 15 seconds -
// leave this here for future use (comment out if not used)
//void scanI2Caddresses() {
//    if(millis() > lastScan + scanInterval){
//        lastScan = millis();
//        
//        bool devicesFound = false;
//        String newDevices = "Devices at: ";
//        //Step through all 127 possible I2C addresses to scan for devices on the I2C bus.
//        for(int i = 1; i < 128; i++){
//            //Make a general call to device at the current address to see if anything responds.
//            Wire.beginTransmission(i);
//            byte status = Wire.endTransmission();
//            if(status == 0){
//                //Device found so append it to our device list event string
//                newDevices.concat(i);
//                newDevices.concat(", ");
//                devicesFound = true;
//            }
//            
//        }
//        if(devicesFound){
//            Log.info(newDevices);
//        }else{
//            Log.info("No Devices Found");
//        }
//    }
//}
