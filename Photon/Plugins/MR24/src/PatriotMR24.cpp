/******************************************************************
PatriotMR24 plugin

Features:
-

http://www.github.com/rlisle/Patriot

This is a Patriot plugin. After making changes use "particle library upload", etc.
 
Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

******************************************************************/

#include "PatriotMR24.h"
#include "IoT.h"

// All this stuff is in case we want to use Rx/Tx instead of S1, S2
#define POLL_INTERVAL_MILLIS 100
#define MESSAGE_HEAD 0x55
#define ACTIVE_REPORT 0x04
#define FALL_REPORT 0x06

#define REPORT_RADAR 0x03
#define REPORT_OTHER 0x05

#define HEARTBEAT 0x01
#define ABNORMAL 0x02
#define ENVIRONMENT 0x05
#define BODYSIGN 0x06
#define CLOSE_AWAY 0x07

#define CA_BE 0x01
#define CA_CLOSE 0x02
#define CA_AWAY 0x03
#define SOMEBODY_BE 0x01
#define SOMEBODY_MOVE 0x01
#define SOMEBODY_STOP 0x00
#define NOBODY 0x00

// Return values
#define DETECTED_NOBODY 0
#define DETECTED_SOMEBODY 50
#define DETECTED_MOVEMENT 100

/**
 * Constructor
 * @param pinNum int pin number that is connected to the sensor output
 * @param name  String name of the event to send when sensor changes
 */
MR24::MR24(int s1pin, int s2pin, String name, String room)
        : Device(name, room)
{
    _type  = 'M';
    _value = 0;
    _s1value = 0;
    _s2value = 0;
    _s1pin = s1pinNum;
    _s2pin = s2pinNum;
}

void MR24::begin() {
//    Serial1.begin(9600);
    _lastPollTime = millis();
    pinMode(_s1pin, INPUT_PULLDOWN);
    pinMode(_s2pin, INPUT_PULLDOWN);
}

/**
 * loop()
 * This is called repeatedly to allow device to do its work.
 */
void MR24::loop()
{
    if (isTimeToCheckSensor())
    {
        if (didSensorChange())
        {
            notify();
        }
    }
}


// Private Helper Methods
/**
 * isTimeToCheckSensor()
 * @return bool if enough time has elapsed to sample switch again
 */
bool MR24::isTimeToCheckSensor()
{
    long currentTime = millis();
    if (currentTime < _lastPollTime + POLL_INTERVAL_MILLIS)
    {
        return false;
    }
    _lastPollTime = currentTime;
    return true;
}


/**
 * didSensorChange()
 *
 * @return bool if switch has changed since last reading
 */
bool MR24::didSensorChange()
{
    int oldS1 = _s1Value;
    int oldS2 = _s2Value;
    _s1value = digitalRead(_s1pin);
    _s2value = digitalRead(_s2pin);
    _value = _s1value ? 25 : 0;
    _value += _s2value ? 50 : 0;
    return (oldS1 != _s1value || olds2 != _s2value);
    
// Example code using Rx/Tx instead of S1, S2
//    int data[14] = {0};
//    int i = 0;
//    int Msg;
//    int newValue = _value;
//
//    Msg = Serial1.read();
//    if(Msg == MESSAGE_HEAD){
//      for(i = 0; i<14; i++){
//        data[i] = Msg;
//        Msg = Serial1.read();
//        if (Msg == MESSAGE_HEAD){
//            newValue = Situation_judgment(data[5], data[6], data[7], data[8], data[9]);
//            continue;
//        }
//        delay(25);
//       }
//     }
//    if(newValue != _value) {
//        _value = newValue;
//        return true;
//    }
//    return false;
}

//void MR24::situation_judgment(int ad1, int ad2, int ad3, int ad4, int ad5){
//  if(ad1 == REPORT_RADAR || ad1 == REPORT_OTHER){
//        if(ad2 == ENVIRONMENT || ad2 == HEARTBEAT){
//          if(ad3 == NOBODY){
//            return NOBODY;
//          }
//          else if(ad3 == SOMEBODY_BE && ad4 == SOMEBODY_MOVE){
//            Serial.println("radar said somebody move");
//          }
//          else if(ad3 == SOMEBODY_BE && ad4 == SOMEBODY_STOP){
//            Serial.println("radar said somebody stop");
//          }
//        }
//        else if(ad2 == CLOSE_AWAY){
//          if(ad3 == CA_BE && ad4 == CA_BE){
//            if(ad5 == CA_BE){
//              Serial.println("radar said no move");
//            }
//            else if(ad5 == CA_CLOSE){
//              Serial.println("radar said somebody close");
//            }
//            else if(ad5 == CA_AWAY){
//              Serial.println("radar said somebody away");
//            }
//          }
//        }
//  }
//}


/**
 * notify()
 * Publish switch state
 */
void MR24::notify()
{
    String topic = "patriot/" + _name;
    String message = String(_value);
    IoT::mqttPublish(topic,message);
}
