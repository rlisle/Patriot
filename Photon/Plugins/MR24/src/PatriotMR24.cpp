/******************************************************************
PatriotMR24 plugin

Features:
- Use Rx/Tx if pins specified as 0,0

http://www.github.com/rlisle/Patriot

This is a Patriot plugin. After making changes use "particle library upload", etc.
 
Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

******************************************************************/

#include "PatriotMR24.h"
#include "IoT.h"

// All this stuff is in case we want to use Rx/Tx instead of S1, S2
#define POLL_INTERVAL_MILLIS 2000
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
#define DETECTED_SOMEBODY_FAR 25
#define DETECTED_SOMEBODY 50
#define DETECTED_SOMEBODY_CLOSE 75
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
    _s1pin = s1pin;
    _s2pin = s2pin;
}

void MR24::begin() {
    _lastPollTime = millis();
    if (usingS1S2()) {
        pinMode(_s1pin, INPUT_PULLDOWN);
        pinMode(_s2pin, INPUT_PULLDOWN);
    } else {
        Serial1.begin(9600);
    }
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
bool MR24::usingS1S2() {
    return (_s1pin > 0 && _s2pin > 0);
}

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
bool MR24::didSensorChange() {
    if(usingS1S2()) {
        return didS1S2sensorChange();
    } else {
        return didTxRxSensorChange();
    }
}

bool MR24::didS1S2sensorChange()
{
    int oldS1 = _s1value;
    int oldS2 = _s2value;
    _s1value = digitalRead(_s1pin);
    _s2value = digitalRead(_s2pin);
    _value = _s1value ? 25 : 0;
    _value += _s2value ? 50 : 0;
    return (oldS1 != _s1value || oldS2 != _s2value);
}

bool MR24::didTxRxSensorChange()
{
    int data[14] = {0};
    int i = 0;
    int length = 0;
    int function = 0;
    int address = 0;
    int chksum = 0;
    int Msg;
    int newValue = _value;

    // This looks wrong. It's using 0x55 start byte of next packet to identify the end of the previous.
    // Also, it is storing the start byte into data[0], but reading data from data[5-9]
    //  but data would be in data[6-10] if data[0] is 0x55.
    Msg = Serial1.read();
    if(Msg == MESSAGE_HEAD){
        delay(25);
        length = Serial1.read();
        delay(25);
        length += Serial1.read() << 8;
        delay(25);
        function = Serial1.read();
        delay(25);
        address = Serial1.read();
        delay(25);
        address += Serial1.read() << 8;
        delay(25);
        for(i=0; i<length-7; i++) {
            data[i] = Serial1.read();
            delay(25);
        }
        chksum = Serial1.read();
        delay(25);
        chksum += Serial1.read() << 8;
        
        newValue = situation_judgment(data[0], data[1], data[2], data[3], data[4]);
        String status = String::format("Status result=%d, length=%d, function=%d, address=%x, data=%d, %d, %d, %d, %d chksum=%x",newValue,length,function,address,data[0],data[1],data[2], data[3], data[4],chksum);
        IoT::mqttPublish("DEBUG:",status);

        
//      for(i = 0; i<14; i++){
//        data[i] = Msg;              // We don't really need to save the 0x55
//        Msg = Serial1.read();
//        if (Msg == MESSAGE_HEAD){
//            newValue = situation_judgment(data[5], data[6], data[7], data[8], data[9]);
//            continue;
//        }
//        delay(25);
//       }
     }
    if(newValue != _value) {
        _value = newValue;
        return true;
    }
    return false;
}

int MR24::situation_judgment(int ad1, int ad2, int ad3, int ad4, int ad5)
{
    if(ad1 == REPORT_RADAR || ad1 == REPORT_OTHER){ // 0x03, 0x05
        if(ad2 == ENVIRONMENT || ad2 == HEARTBEAT){ // 0x05, 0x01
            if(ad3 == NOBODY){                      // 0x00
                return DETECTED_NOBODY;             // 0
            }
            else if(ad3 == SOMEBODY_BE && ad4 == SOMEBODY_MOVE){
                Serial.println("radar said somebody move");
                return DETECTED_MOVEMENT;
            }
            else if(ad3 == SOMEBODY_BE && ad4 == SOMEBODY_STOP){
                Serial.println("radar said somebody stop");
                return DETECTED_SOMEBODY;
            }
        }
        else if(ad2 == CLOSE_AWAY){
            if(ad3 == CA_BE && ad4 == CA_BE){
                if(ad5 == CA_BE){
                    Serial.println("radar said no move");
                    return DETECTED_SOMEBODY;
                }
                else if(ad5 == CA_CLOSE){
                    Serial.println("radar said somebody close");
                    return DETECTED_SOMEBODY_CLOSE;
                }
                else if(ad5 == CA_AWAY){
                    Serial.println("radar said somebody away");
                    return DETECTED_SOMEBODY_FAR;
                }
            }
        }
    }
    return DETECTED_NOBODY;
}


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
