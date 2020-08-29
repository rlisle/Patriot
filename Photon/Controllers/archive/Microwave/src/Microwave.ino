/*
 * Microwave Lights Controller
 *
 * Descriiption: This sketch controls the lights above the microwave.
 * It will trigger based on proximity and light level.
 *
 * Author: Ron Lisle
 *
 * 1. Monitor proximity.
 * 2. Monitor light level
 * 3. Adjust lights
 *
 * Hardware
 * 1. Photon
 * 2. 5x5 Photon IoT v2 PCB
 * 3. 2 x LED driver board (kLed2Pin, kLed3Pin)
 *    - Microwave control panel recessed light
 *    - Cabinet lights (maybe ribbon lights?)
 * 4. Ultrasonic proximity detector (k5vIo2Pin, k5vIo3Pin)
 * 5. Light level sensor (kPhotoPin A4)
 * 6. DHT11 temperature/humidity sensor (k3vIo1Pin)
 *
 * History
 *  3/16/17 Convert to IoT library
 */
#include <IoT.h>
IoT*     iot;

void setup() {
  iot = IoT::getInstance();
  iot->setControllerName("Microwave");
  iot->setPublishName("lislerv");
  iot->begin();

  iot->addLight(RX, "MicrowaveSpots");
  iot->addLight(TX, "MicrowaveCabinet");

  iot->addBehavior("MicrowaveSpots", new Behavior("away",'>',0, 100));
  iot->addBehavior("MicrowaveSpots", new Behavior("coffee",'>',0, 5));
  iot->addBehavior("MicrowaveSpots", new Behavior("cook",'>',0, 100));
  iot->addBehavior("MicrowaveSpots", new Behavior("dishes",'>',0, 100));
  iot->addBehavior("MicrowaveSpots", new Behavior("kitchen",'>',0, 100));

  iot->monitorPresence(A1, A0, 3, 36, "kitchen");
  iot->monitorTemperature(A2, DHT22, "kitchen", 5000);
}

void loop() {
  iot->loop();
}
