/* 
 * Project BedBath
 * Author: Ron Lisle
 * Date: 1/1/24
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);

int light = D7;

// setup() runs once, when the device is first turned on
void setup() {
  pinMode(light, OUTPUT);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // Example: Publish event to cloud every 10 seconds. Uncomment the next 3 lines to try it!
  // Log.info("Sending Hello World to the cloud!");
  // Particle.publish("Hello world!");
  // delay( 10 * 1000 ); // milliseconds and blocking - see docs for more info!
  digitalWrite(light, HIGH);
  delay(1000);
  digitalWrite(light, LOW);
  delay(1000);
}
