/******************************************************************
 HueLight plugin

 Features:
 - Control Philips Hue light.

http://www.github.com/rlisle/Patriot
 
Example code used from https://www.digikey.com/en/maker/blogs/2019/how-to-post-data-using-the-particle-photon

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

******************************************************************/

#include "HueLight.h"
#include "IoT.h"

/**
 * Constructor
 * @param name  String name of the checklist item
 */
HueLight::HueLight(String name, String room, byte server[4], String userid)
        : Device(name, room)
{
    _server = server;
    _userID = userid;
    _value = 0;
    _type  = 'L';
}

void HueLight::begin(byte server[]) {
    if(_client.connect(_server,80)) {
        Log("HueLight "+_name+" connected");
        _client.stop();
    } else {
        Log("HueLight "+_name+" failed to connect!");
    }
    
}

/**
 * loop()
 * This is called repeatedly to allow device to do its work.
 */
void HueLight::loop()
{
    //Nothing to do?
}

/**
 * Set value
 * @param value Int 0 to 100
 */
void HueLight::setValue(int value) {
    Log.info("HueLight " + _name + " setValue: " + String(value));
    if(_targetValue == value) {
        Log.info("HueLight value already set, but writing again!");
    }
    writeToHue();
}


// Private Helper Methods
void HueLight::writeToHue() {
    //TODO: may try staying connected if noticably faster?
    if(_client.connect(server,80)) {
        _client.println("PUT /api/" + _user + "/lights/" + _name + "/state");
        if(_value > 0) {
            _client.println('{"on":"true"}');
        } else {
            _client.println('{"on":"false"}');
        }
    } else {
        Log("HueLight "+_name+" not connected");
    }
    
}

/**
 * notify()
 * Publish switch state
 */
void HueLight::notify()
{
    String topic = "patriot/" + _name;
    String message = String(_value);
    IoT::mqttPublish(topic,message);
}
