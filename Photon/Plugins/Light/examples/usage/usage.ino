// Example usage for Patriot-Light library by Ron Lisle.

#include "Patriot-Light.h"

// Initialize objects from the lib
PatriotLight patriotLight;

void setup() {
    // Call functions on initialized library objects that require hardware
    patriotLight.begin();
}

void loop() {
    // Use the library's initialized objects and functions
    patriotLight.process();
}
