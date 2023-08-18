/*******************************************************
Custom Event Handler example

This example demonstrates how to subscribe to Patriot events.
It allows you to respond to Alexa device commands.

It is based on the starter example with behaviors removed.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-10-30: Initial creation based on starter example.
********************************************************/

#include <IoT.h>
#include <PatriotLight.h>

extern String publishNameVariable;

IoT *iot;

void myEventHandler(const char *eventName, const char *rawData)
{
    // Convert arguments to Strings because their easier to work with
    String data(rawData);
    String event(eventName);
    Serial.println("myEventHandler received event: " + event + ", data: " + data);

    // Split "event:percent" formatted data into name & percent components
    int colonPosition = data.indexOf(':');
    String name = data.substring(0,colonPosition);
    String percent = data.substring(colonPosition+1);
    Serial.println("myEventHandler event name: " + name + ", percent: " + percent);
}

void setup()
{
    // Example of how to receive events to do your own processing
    bool result = Particle.subscribe(publishNameVariable, myEventHandler, MY_DEVICES);
    if(result == false)
    {
        Serial.println("Failed to subscribe to myEventHandler");
    } else {
        Serial.println("myEventHandler subscribe succeeded");
    }

    iot = IoT::getInstance();
    iot->begin();

    Light *led = new Light(D7, "Blue");
    iot->addDevice(blue);
}

void loop() {
    iot->loop();
}
