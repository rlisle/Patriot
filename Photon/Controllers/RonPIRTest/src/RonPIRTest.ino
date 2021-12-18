/*
 * RonPIRTest Controller
 *
 * Description: This sketch is for testing a PIR device connected to D0
 * Refer to www.ElectronicWings.com/particle/pir-sensor-interfacing-with-particle-photon
 *
 * Author: Ron Lisle
 *
 * Hardware
 * - built-in blue LED     D7
 * - PIR                   D0
 */

int PIR_PIN = D0;
int warm_up = 1;
int current_state = 0;

void setup() {
    pinMode(PIR_PIN, INPUT);
    Serial.begin(57600);    // Use "screen /dev/tty.usbmodem21201 57600" or 9600, etc.
}

void loop() {

    int sensor_output;
    sensor_output = digitalRead(PIR_PIN);
    if(warm_up == 1) {
        Serial.println("Warming Up");
        warm_up = 0;
        delay(2000);
        
    } else if(sensor_output != current_state) {
        current_state = sensor_output;
        if(sensor_output == LOW) {
            Serial.println("No object in sight");
        } else {
            Serial.println("Objected Detected");
        }
        delay(1000);
    }
}
