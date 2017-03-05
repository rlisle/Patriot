// Example usage for IoT library by Ron Lisle.

#include <IoT.h>

// Include the IoT functionality in your sketch.
IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("lislerv");
    iot->setPublishName("RonTest");
    iot->begin();
}

void loop() {
    long currentMillis = millis();
    iot->loop(currentMillis);
}
