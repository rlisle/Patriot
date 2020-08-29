/*
 * Refrigerator Fan Controller
 *
 * This sketch controls the fan located behind the refrigerator.
 *
 * 1. Read DHT11 temp sensor
 * 2. Control fan based on the read temperature
 * 3. Publish temp changes
 * 4. Publish fan speed changes (Why?)
 *    If this is needed, perhaps expose a variable instead?
 *
 * Note: this will require enhancing behaviors to respond to
 *       different values of events (temperature)
 *
 * Hardware
 * 1. Photon
 * 2. 5x5 Photon 1st gen board
 * 3. DHT11 temp/humidity sensor (D1)
 *    Left to right: +5/3.3, Data In, n/c, Gnd
 * 4. Fan motor control PWM (D0)
 *    LED Driver board
 * 5. Future options
 *    a. Additional DHT11 for outside and refer inside temps (D2-D4)
 *    b. Inside refer light
 */

#include <IoT.h>
IoT      *iot;

void setup() {
  iot = IoT::getInstance();
  iot->setControllerName("ReferFan");
  iot->setPublishName("patriot");
  iot->begin();

  iot->addFan(D0, "ReferFan");

  iot->addBehavior("ReferFan", new Behavior("ReferRearTemp",'>',80, 25));
  iot->addBehavior("ReferFan", new Behavior("ReferRearTemp",'>',90, 100));

  iot->monitorTemperature(D1, DHT11, "ReferRearTemp", 5000);

}

void loop() {
  iot->loop();
}
