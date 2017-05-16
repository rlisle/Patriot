// Example usage for Patriot-Switch library by Ron Lisle.

#include "Patriot-Switch.h"

// Initialize objects from the lib
PatriotSwitch patriotSwitch;

void setup() {
    // Call functions on initialized library objects that require hardware
    patriotSwitch.begin();
}

void loop() {
    // Use the library's initialized objects and functions
    patriotSwitch.process();
}
