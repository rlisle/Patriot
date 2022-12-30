/**
 Patriot Power Usage Monitoring

 This plugin supports monitoring of the power usage reported via MQTT by Shelly device
 
 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:
 Shelly Power Monitor

 Changelog:
 2022-12-29: Initial creation
 */

#pragma once

#include "Particle.h"
#include <device.h>

class Power : public Device
{
 private:
    
    void    parsePowerMessage(String lcTopicEnd, String lcMessage);

    void    notify();

 public:
    Power(String name, String room);
    
    void    begin();
    void    loop();
    void    mqtt(String topic, String message);
};
