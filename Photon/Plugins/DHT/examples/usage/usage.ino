// Example usage for Patriot-DHT library by Ron Lisle.

#include "Patriot-DHT.h"

// Initialize objects from the lib
PatriotDHT patriotDHT;

void setup() {
    // Call functions on initialized library objects that require hardware
    patriotDHT.begin();
}

void loop() {
    // Use the library's initialized objects and functions
    patriotDHT.process();
}
