/** Piano Cabinet
 *
 * This sketch controls the Photon mounted in the piano cabinet.
 * It manages
 *    - motorized piano drawer (someday)
 *    - lights inside piano cabinet?
 *
 * It provides a function to be called to open or close the drawer.
 * Optionally, it may provide an indicator of the current drawer position.
 *
 * curl https://api.spark.io/v1/devices/430033000f47343339383037/command
 *       -d access_token=3030a462f44ce7c80b2eabadd4a0f7f3c3fc6233
 *       -d params=piano=open
 *
 * Hardware
 * 1. Photon
 * 2. 5x5min Photon IoTv2
 * 3. Stepper 1 for drawer motors
 * 4. LED driver for cabinet lights
 */

/****************/
/*** INCLUDES ***/
/****************/
#include "LisleRV.h"
#include "activities.h"
#include "alive.h"
#include "lights.h"
#include "stepper.h"

/*****************/
/*** CONSTANTS ***/
/*****************/
#define kCabinetLightsPin  kLed2Pin

#define  kStepDelay              2        // 2ms per step
#define  kStepsPerRevolution     2048     // 32 steps * 64:1 gear ratio
#define  kNumRackTeeth           19 * 4   // 76
#define  kNumPinionTeeth         24
#define  kTotalRevolutions       kNumRackTeeth / kNumPinionTeeth // 3.167
#define  kTotalStepsWhenOpen     kStepsPerRevolution * kTotalRevolutions // 6,486

/*****************/
/*** VARIABLES ***/
/*****************/
Alive      alive(kAliveInterval, String("BoothAlive"));
Lights     lights;
Activities activities;
Stepper    stepper(0);

Light     *cabinetLights;

/*************/
/*** SETUP ***/
/*************/
void setup() {
  Particle.publish("DEBUG", "Piano cabinet controller starting...");
  Particle.subscribe("lislerv",subscribeHandler);
  setupSerial();
  setupLights();
  setupActivities();
  stepper.performStartupCycle();
}

void setupSerial() {
    Serial.begin(57600);
    Serial.println(F("PianoCabinet Starting..."));
}

void setupLights() {
  cabinetLights = lights.addLight(kCabinetLightsPin, "piano cabinet lights");
}

void setupActivities() {
  activities.addActivity("piano",cabinetLights,"on");
}

void loop() {
    currentTime = millis();
    switches.loop();
    alive.pollAlive();
    lights.loop();
    stepper.loop();
    handleSerial();
}

/*************************/
/*** Subscribe Handler ***/
/*************************/
void subscribeHandler(const char *eventName, const char *rawData) {

  String data(rawData);   // This apparently converts the data somehow
  int colonPosition = data.indexOf(':');
  String name = data.substring(0,colonPosition);
  String state = data.substring(colonPosition+1);

  Light* light = lights.getLightWithName(name);
  if(light) {
    light->set(state);

  } else {
    activities.performActionsForName(name,state);

  }
}

void handleSerial() {
    // For test & debugging, accept some simple commands over serial
    // For now, super simple, 1 character commands:
    //  o = open piano
    //  c = close piano
    //  ? = display help text
    char byteReceived;
    if(Serial.available()) {
        byteReceived = Serial.read();
        switch(byteReceived) {
        case 'c':
            Serial.println("Closing piano...");
            closePiano();
            break;

        case 'o':
            Serial.println("Opening piano...");
            openPiano();
            break;

        case '?':
            Serial.println("o = open piano\nc = close piano\n? = display this message\n");
            break;

        default:
            Serial.println("Input not recognized/supported");
        }
    }
}
