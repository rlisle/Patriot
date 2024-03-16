/**
 Zigbee contact sensor control
 for use with Sonoff USB Zigbee adapter

 Features:
 - Magnetic contact detection

 Leverage capability of Sonoff SNZB-04 Wireless Door/Window Sensor.
 
 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 */

#include "IoT.h"

/**
 * Constructor
 * @param name String name used to address the light.
 */
ZigbeeContact::ZigbeeContact(String name, String room, void (*handler)(int,int))
        : Device(name, room, 'D', handler)
{
    // Nothing to do
}

void ZigbeeContact::begin() {
    // Nothing to do
}

void ZigbeeContact::mqtt(String topic, String message) {
    // Parse patriot/zigbee/<device> 
    // {"battery":100,"battery_low":false,"contact":true/false,"linkquality":156,"tamper":false,"voltage":3200}
    String subtopics[5];
    int start = 0;
    int end = topic.indexOf('/');
    int numTopics = 0;
    if(end > 0) { // Might be -1 if only 1 subtopic
        do {
            start = end+1;
            end = topic.indexOf('/', start);
            numTopics++;
        } while(numTopics < 4 && end > 0);
    }
    subtopics[numTopics++] = topic.substring(start);  // Last one
    
    if(numTopics == 2 && subtopics[0] == "zigbee") {
        Log.info("DEBUG: zigbee message received");
        if(subtopics[1] == name()) {  
            Log.info("Zigbee message to us");
            //TODO:
        }
    }  
}

/**
 * loop()
 * Called periodically to perform dimming, polling, etc.
 * but this is done by the Sengled light bulb itself.
 */
void ZigbeeContact::loop()
{
    //Nothing to do
};
