// Example usage for Patriot-Fan library by Ron Lisle.

#include "Patriot-Fan.h"

// Initialize objects from the lib
PatriotFan patriotFan;

void setup() {
    // Call functions on initialized library objects that require hardware
    patriotFan.begin();
}

void loop() {
    // Use the library's initialized objects and functions
    patriotFan.process();
}
