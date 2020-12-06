/**
Actuators collection

This object manages a collection of Actuator objects.

Actuators are used by Activities to turn off other Activities
when they become active.
 
http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.
*/

#include "IoT.h"
#include "actuators.h"
#include "constants.h"

Actuators::Actuators() {
    _actuators = NULL;
}

Actuator *Actuators::addActuator(String name, int value) {
    Actuator *actuator = getActuatorWithName(name);
    if (actuator == NULL) {
        log("Actuators addActuator adding " + name + " = " + String(value));
        actuator = new Actuator(name,value);
        if(_actuators == NULL) {
            log("  first actuator");
            _actuators = actuator;
        } else {
            Actuator* ptr = _actuators;
            while(ptr->_next != NULL) ptr = ptr->_next;
            ptr->_next = actuator;
        }
    } else {    // Actuator already exists
        log("Actuators addActuator updating " + name + " = " + String(value) + ", was " + String(actuator->_value));
        actuator->_value = value;
    }
    log("addActuator actuator was added. Count = " + String(count()));
    return actuator;
}

Actuator *Actuators::getActuatorWithName(String name) {
    Actuator *ptr = _actuators;
    while(ptr != NULL) {
        if (ptr->_name.equalsIgnoreCase(name)) {
            log("getActuatorWithName " + name + " found");
            return ptr;
        }
        ptr = ptr->_next;
    }
    log("getActuatorWithName " + name + " not found");
    return NULL;
}

Actuator *Actuators::getActuatorAt(int index)
{
    Actuator *ptr = _actuators;
    for(int x = 0; x<index && ptr->_next !=NULL; x++) {
        ptr = ptr->_next;
    }
    return ptr;
}


int Actuators::count() {
    int i = 0;
    for(Actuator* ptr = _actuators; ptr != NULL; ptr = ptr->_next) i++;
    return i;
}

void Actuators::log(String message, PLogLevel logLevel)
{
    IoT* iot = IoT::getInstance();
    iot->log(message, logLevel);
}

