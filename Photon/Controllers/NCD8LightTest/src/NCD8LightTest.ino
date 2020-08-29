/*
 * NCD8LightTest
 * Description: Turn on a PWM light
 * Author: Ron Lisle
 * Date: 02/23/20
 *
 * To update Photon:
 *   1. Edit this code
 *   2. "particle flash RearPanel"
 *
 * Hardware
 * 1. Photon
 * 2. ControlEverything.com NCD8Relay Photon Controller
 * 3. NCD8PWM board connected via I2C to NCD8Relay board
 * NCD 8-channel PWM OC board with adress switches set per ADDRESS
 * LED connected to channel 0
 * 
 * Note that the PCA9634 connects to 8 BUK98150-55A power FETS.
 * !OE is hardwired to ground, so OE always == 0 (outputs enabled)
 *
 * History
 * 2/23/20 Initial creation
 */

#define ADDRESS  0x40  // high jumper set (address 64)

#include "Particle.h"

void setup() {
    int retries = 3;
    byte status;

    Serial.println("Connect USB then determine address: ls /dev/tty.usb*");
    Serial.println("Connect screen /dev/tty.usbmodem* 56400");
    delay(2000);

    if(!Wire.isEnabled()) {
        Serial.println("Starting wire");
        Wire.begin();
    } else {
      Serial.println("Uh oh, wire was already enabled");
    }

    do {
        Wire.beginTransmission(ADDRESS);
        status = Wire.endTransmission();

    } while( status != 0 && retries-- > 0);

    if(status == 0) {
      Serial.println("Status = 0");
        Wire.beginTransmission(ADDRESS);
        Wire.write(0);      // Mode1 register
        Wire.write(0);      // Osc on, diable AI, subaddrs, and allcall
        Wire.endTransmission();

        Wire.beginTransmission(ADDRESS);
        Wire.write(1);      // Mode2 register
        Wire.write(0x04);   // Dimming, Not inverted, totem-pole
        Wire.endTransmission();

    } else {
        Serial.println("Transmission failed");
    }

    Wire.beginTransmission(ADDRESS);
	  Wire.write(2);    // 2 + led #
	  Wire.write(128);  // 1/2 brightness
	  status = Wire.endTransmission();
    if(status != 0){
      Serial.println("Write failed");
    }else{
      Serial.println("Write succeeded");
    }    
}

// void loop() {
//     bool devicesFound = false;
//     String newDevices;

//     Serial.println("Starting loop...");
//     //Step through all 127 possible I2C addresses to scan for devices on the I2C bus.
//     for(int i = 1; i < 128; i++){
//         //Make a general call to device at the current address to see if anything responds.
//         Wire.beginTransmission(i);
//         byte status = Wire.endTransmission();
//         if(status == 0){
//             //Device found so append it to our device list event string
//             // Devicess found at 1, 3, 32, 112 (0x01, 0x03, 0x20, 0x70)
//             char devices[32];
//             sprintf(devices, "Device at: %ix, ", i);
//             newDevices.concat(devices);
//             devicesFound = true;
//         }
//     }
//     if(devicesFound){
//         Serial.println(newDevices);
//     }else{
//         Serial.println("No devices found");
//     }
    
//     delay(2000);
// }

// From TravisE_NCD_Technica
unsigned long scanInterval = 10000;
unsigned long lastScan;

void loop() {
    if(millis() > lastScan + scanInterval){
        lastScan = millis();
        // bool devicesFound = false;
        // String newDevices = "Devices at: ";
        // //Step through all 127 possible I2C addresses to scan for devices on the I2C bus.
        // for(int i = 1; i < 128; i++){
        //     //Make a general call to device at the current address to see if anything responds.
        //     Wire.beginTransmission(i);
        //     byte status = Wire.endTransmission();
        //     if(status == 0){
        //         //Device found so append it to our device list event string
        //         newDevices.concat(i);
        //         newDevices.concat(", ");
        //         devicesFound = true;
        //     }
        
        // }
        // if(devicesFound){
        //     Serial.println(newDevices);
        // }else{
        //     Serial.println("No Devices Found");
        // }
    }
}
