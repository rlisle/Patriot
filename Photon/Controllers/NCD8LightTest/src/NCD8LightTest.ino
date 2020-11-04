/*
 * NCD8LightTest
 * Description: Turn on a PWM light
 * Author: Ron Lisle
 * Date: 02/23/20
 *
 * To update Photon:
 *   1. Edit this code
 *   2. "particle flash RonTest"
 *
 * Hardware
 * 1. Photon
 * 2. ControlEverything.com NCD Photon Screw Terminal Breakout board
 * 3. NCD8PWM board connected via I2C to NCD STB board
 * NCD 8-channel PWM OC board with adress switches set per ADDRESS
 * LED connected to channel 0
 * 
 * Note that the PCA9634 connects to 8 BUK98150-55A power FETS.
 * !OE is hardwired to ground, so OE always == 0 (outputs enabled)
 *
 * History
 * 2/23/20 Initial creation
 * 11/3/20 Modify for STB board
 */

#define ADDRESS  1 // low jumper set (address 1)

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

    retries = 3;
    do {
        Wire.beginTransmission(ADDRESS);
        status = Wire.endTransmission();

    } while( status != 0 && retries-- > 0);

    if(status == 0) {
      Serial.println("Status = 0, setting Mode1 and Mode2");
        Wire.beginTransmission(ADDRESS);
        Wire.write(0);      // Control register - No AI, point to reg 0 Mode1
        Wire.write(0);      // Mode1 register: Osc on (no sleep), diable AI, subaddrs, and allcall
        Wire.endTransmission();

        Wire.beginTransmission(ADDRESS);
        Wire.write(1);      // Mode2 register
        Wire.write(0x04);   // Dimming, Not inverted, totem-pole (this is the default except D0)
        Wire.endTransmission();

    } else {
        Serial.println("Transmission failed");
    }

    Serial.println("Turning ON all LEDS");
    Wire.beginTransmission(ADDRESS);
	  Wire.write(0x8c); // AI + LEDOUT0
	  Wire.write(0x55); // LEDOUT0 All 4 LEDS ON
	  Wire.write(0x55); // LEDOUT1 All 4 LEDS ON
	  status = Wire.endTransmission();
    if(status != 0){
      Serial.println("Write failed");
    }else{
      Serial.println("Write succeeded");
    }    

    delay(2000);

    Serial.println("Turning OFF all LEDS");
    Wire.beginTransmission(ADDRESS);
	  Wire.write(0x8c); // AI + LEDOUT0
	  Wire.write(0x00); // LEDOUT0 All 4 LEDS OFF
	  Wire.write(0xaa); // LEDOUT1 All 4 LEDS Dimming
	  status = Wire.endTransmission();
    if(status != 0){
      Serial.println("Write failed");
    }else{
      Serial.println("Write succeeded");
    }    

//    delay(2000);

    // Serial.println("Setting LED 2 = 255");
    // Wire.beginTransmission(ADDRESS);
	  // Wire.write(3);    // 2 + led #
	  // Wire.write(255);  // Full brightness
	  // status = Wire.endTransmission();
    // if(status != 0){
    //   Serial.println("Write failed");
    // }else{
    //   Serial.println("Write succeeded");
    // }    

    // delay(1000);

    // Serial.println("Setting LED 2 = 0");
    // Wire.beginTransmission(ADDRESS);
	  // Wire.write(3);    // 2 + led #
	  // Wire.write(0);    // Off
	  // status = Wire.endTransmission();
    // if(status != 0){
    //   Serial.println("Write failed");
    // }else{
    //   Serial.println("Write succeeded");
    // }    

    // Repeat
    // TODO:
}

// From TravisE_NCD_Technica
unsigned long scanInterval = 10000;
unsigned long lastScan;

void loop() {
    byte status;
    // if(millis() > lastScan + scanInterval){
    //     lastScan = millis();

    //     bool devicesFound = false;
    //     String newDevices = "Devices at: ";
    //     //Step through all 127 possible I2C addresses to scan for devices on the I2C bus.
    //     for(int i = 1; i < 128; i++){
    //         //Make a general call to device at the current address to see if anything responds.
    //         Wire.beginTransmission(i);
    //         byte status = Wire.endTransmission();
    //         if(status == 0){
    //             //Device found so append it to our device list event string
    //             newDevices.concat(i);
    //             newDevices.concat(", ");
    //             devicesFound = true;
    //         }
        
    //     }
    //     if(devicesFound){
    //         Serial.println(newDevices);
    //     }else{
    //         Serial.println("No Devices Found");
    //     }
    // }

    Serial.println("Turning ON LEDS 0-3");
    Wire.beginTransmission(ADDRESS);
	  Wire.write(0x8c); // AI + LEDOUT0
	  Wire.write(0x55); // LEDOUT0 All 4 LEDS ON
	  //Wire.write(0x55); // LEDOUT1 All 4 LEDS ON
	  status = Wire.endTransmission();
    if(status != 0){
      Serial.println("Write failed");
    }else{
      Serial.println("Write succeeded");
    }    

    Serial.println("Setting LEDs 4-7 = 255");
    Wire.beginTransmission(ADDRESS);
	  Wire.write(0x86); // AI + 2 + led # 4
	  Wire.write(255);  // Full brightness
	  Wire.write(255);  // Full brightness
	  Wire.write(255);  // Full brightness
	  Wire.write(255);  // Full brightness
	  status = Wire.endTransmission();
    if(status != 0){
      Serial.println("Write failed");
    }else{
      Serial.println("Write succeeded");
    }    

    delay(1000);

    Serial.println("Turning OFF LEDS 0-3");
    Wire.beginTransmission(ADDRESS);
	  Wire.write(0x8c); // AI + LEDOUT0
	  Wire.write(0x00); // LEDOUT0 All 4 LEDS OFF
	  //Wire.write(0x00); // LEDOUT1 All 4 LEDS OFF
	  status = Wire.endTransmission();
    if(status != 0){
      Serial.println("Write failed");
    }else{
      Serial.println("Write succeeded");
    }    

    Serial.println("Setting LEDs 4-7 = 0");
    Wire.beginTransmission(ADDRESS);
	  Wire.write(0x86);    // 2 + led # 4
	  Wire.write(0);
	  Wire.write(0);
	  Wire.write(0);
	  Wire.write(0);
	  status = Wire.endTransmission();
    if(status != 0){
      Serial.println("Write failed");
    }else{
      Serial.println("Write succeeded");
    }    

    delay(1000);

}
