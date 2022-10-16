/*
 * NCD16LightTest
 * Description: Turn on a PWM light
 * Author: Ron Lisle
 * Date: 10/1/22
 *
 * To update Photon:
 *   1. Edit this code
 *   2. "particle flash OfficeTest"
 *
 * Hardware
 * 1. Photon
 * 2. ControlEverything.com NCD Photon Screw Terminal Breakout board
 * 3. NCD16PWM board connected via I2C to NCD STB board
 * NCD 8-channel PWM OC board with adress switches set per ADDRESS
 * LED connected to channel 0
 * 
 * Note that the PCA9685 connects to 16 BUK98150-55A power FETS.
 * !OE is hardwired to ground, so OE always == 0 (outputs enabled)
 *
 * Programming the PCA9685 involves setting the PWM low and high durations, not just a simple level value.
 *
 * PCA9685 doc: https://media.ncd.io/sites/2/20170721134542/PCA9685.pdf
 *
 * History
 * 10/1/22 Initial creation
 */

//#include <IoT.h>
//#include <PatriotNCD16Dimmer.h>

#define CONTROLLER_NAME "OfficeTest"
#define MQTT_BROKER "192.168.50.33"
#define ADDRESS 0x41      // PWM board address low (0) switch on

//SYSTEM_THREAD(ENABLED);
//SYSTEM_MODE(SEMI_AUTOMATIC);

int  address = 0x41;
system_tick_t lastAliveTime = 0;
bool lightState = false;
bool initialized = false;
int  lightNum = 0;

void setup() {
    Serial.begin();
    Serial.println("NCD16LightTest setup");
    
    WiFi.selectAntenna(ANT_INTERNAL);
    WiFi.useDynamicIP();
//    IoT::begin(MQTT_BROKER, CONTROLLER_NAME);
//    createDevices();

    lastAliveTime = Time.now(); // Wait 15 seconds

 }

//void createDevices() {
//    Device::add(new NCD16Dimmer(ADDRESS, 0, "SpotLight", "Office", 2));
//}

void loop() {
//    IoT::loop();

    // Display an alive message every 15 seconds
    sendAlivePeriodically();
    

}

void sendAlivePeriodically() {
    system_tick_t secondsSinceLastAlive = Time.now() - lastAliveTime;
    if(secondsSinceLastAlive > 15) {
        lastAliveTime = Time.now();
        String time = Time.format(Time.now(), "%a %H:%M");
        Serial.println("Alive "+ time);
        if(initialized) {
            Serial.println("Is initialized so toggling");
            toggleLight();
        } else {
            Serial.println("Initializing...");
            initialize();
        }
    }
}

void toggleLight() {
    
    int current4k = 0;
    int retryCount = 3;
    byte status;
    int lsb;
    int msb;
    int reg = 6 + (lightNum * 4);   // This is 0 based

    if(lightState == false) {
        lightState = true;
        
        current4k = 0x0fff; // 12 bits all on
        lsb = current4k & 0xff;
        msb = current4k >> 8;
        do {
            Serial.println("Writing led toggle ON value");
            Wire.beginTransmission(address);
            Wire.write(reg);
            Wire.write(0);      // Delay lsb
            Wire.write(0);      // Delay msb
            Wire.write(lsb);    // Off lsb
            Wire.write(msb);    // Off msb
            status = Wire.endTransmission();
            retryCount--;
        } while(status != 0 && retryCount > 0);

    } else {
        lightState = false;

        current4k = 0;
        lsb = 0;
        msb = 0;
        do {
            Serial.println("Writing led toggle OFF value");
            Wire.beginTransmission(address);
            Wire.write(reg);
            Wire.write(0);      // Delay lsb
            Wire.write(0);      // Delay msb
            Wire.write(lsb);    // Off lsb
            Wire.write(msb);    // Off msb
            status = Wire.endTransmission();
            retryCount--;
        } while(status != 0 && retryCount > 0);
    }
    if(status != 0) {
        Log.error("toggleLight write failed for light "+String(lightNum)+", level = "+String(current4k));
//        reset();
    } else {
        // Read back value written for debugging
        int reg = 8 + (lightNum * 4);   // This is 0 based
        retryCount = 3;
        do {
            Serial.println("Reading led value");
            Wire.beginTransmission(address);
            Wire.write(reg);
            Wire.write(0);      // Delay lsb
            Wire.write(0);      // Delay msb
            Wire.write(lsb);    // Off lsb
            Wire.write(msb);    // Off msb
            status = Wire.endTransmission();
            retryCount--;
        } while(status != 0 && retryCount > 0);

        
    }
}

void initialize() {
    
    int retries = 0;
    
     Wire.begin();
     
     //Open connection to specified address
retryAddress:
     Wire.beginTransmission(address);
     byte status = Wire.endTransmission();
     if(status != 0){
         if(retries < 3){
             retries++;
             Serial.println("Set Address Command failed, retrying");
             goto retryAddress;
         }else{
             Serial.println("Set Address Command failed");
             retries = 0;
         }

     }else{
         Serial.println("Set Address Command Successful");
         Wire.beginTransmission(address);
         Wire.write(254);       // Prescale
         Wire.write(5);         // osc_clock / (4096 * rate)
         Wire.endTransmission();
         Wire.beginTransmission(address);
         Wire.write(0);         // Mode1
         Wire.write(161);       // 0xA1 = Restart En, AI, SubAdds dis,All Call en
         Wire.endTransmission();
         initialized = true;
         Serial.println("Initialization successful");
     }
}
