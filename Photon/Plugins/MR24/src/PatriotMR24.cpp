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
#define POLL_INTERVAL_MILLIS 500
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
#define DETECTED_NOTHING -1
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
    _value = DETECTED_NOTHING;
    _s1value = 0;
    _s2value = 0;
    _s1pin = s1pin;
    _s2pin = s2pin;
    _index = 0;
    _statusMessage = "Init";
    _prevStatusMessage = "Init";
}

void MR24::begin() {
    _lastPollTime = millis();
    if (usingS1S2()) {
        pinMode(_s1pin, INPUT_PULLDOWN);
        pinMode(_s2pin, INPUT_PULLDOWN);
    } else {
        Serial1.begin(9600);    // 9600 baud, default 8 bits, no parity, 1 stop bit SERIAL_8N1
        sendScene(3);       // bedroom
        sendSensitivity(8); // default 7
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
    if(!usingS1S2()) return true;
    
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
    _value = _s1value ? DETECTED_SOMEBODY_FAR : 0;
    _value += _s2value ? DETECTED_SOMEBODY : 0;
    return (oldS1 != _s1value || oldS2 != _s2value);
}

bool MR24::didTxRxSensorChange()
{
    int Msg;
    int prevValue = _value;

    while(Serial1.available()) {
        Msg = Serial1.read();
        if(Msg == MESSAGE_HEAD && _index > 0){  // Skip first time
            _index = 0;             // Loop back to start of buffer
            int newValue = situation_judgment(_data[5], _data[6], _data[7], _data[8], _data[9]);
            if(newValue > DETECTED_NOTHING) {
                _value = newValue;
            }
        }
        if(_index < 14) {
            _data[_index++] = Msg;
        }
    }
    return _value != prevValue;
}


void MR24::logMessage() {
    int length = 0;
    int function = 0;
    int address1 = 0;
    int address2 = 0;

    length = _data[1] + (_data[2] << 8);
    function = _data[3];
    address1 = _data[4];
    address2 = _data[5];
    // Data in data[6-10]
    //TODO: account for length
    //TODO: get and check CRC

//    if(function == 4) {
//        String proactive = "Radar ";
//        if(address1 == 1) {
//            proactive += "module ID ";  // address2 s/b 2
//        } else if(address1 == 3){
//            if(address2 == 5) {
//                if(_data[6]==0 && _data[7]==255 && _data[8] == 255) proactive += "unoccupied ";
//                if(_data[6]==1 && _data[7]==0 && _data[8] == 255) proactive += "stationary ";
//                if(_data[6]==1 && _data[7]==1 && _data[8] == 1) proactive += "people moving ";
//            }
//            if(address2 == 6) {
//                return;
//                //proactive += "radar motor (float) ";
//            }
//            if(address2 == 7) {
//                if(_data[6]==1 && _data[7]==1 && _data[8] == 1) proactive += "approaching away none ";
//                if(_data[6]==1 && _data[7]==1 && _data[8] == 2) proactive += "approaching away close ";
//                if(_data[6]==1 && _data[7]==1 && _data[8] == 3) proactive += "approaching stay away ";
//            }
//        } else if(address1 == 5) {
//            if(address2 == 1) {
//                //if(_data[6]==0 && _data[7]==255 && _data[8] == 255) proactive += "Radar Heartbeat unoccupied ";
//                if(_data[6]==1 && _data[7]==0 && _data[8] == 255) proactive += "heartbeat stationary ";
//                if(_data[6]==1 && _data[7]==1 && _data[8] == 1) proactive += "heartbeat people moving ";
//            }
//        } else {
//            proactive += "address = " + String(address1);
//        }
//        Log.info(proactive);
//
//    } else {
        String status = String::format("Status %d, len=%d, func=%d, a1=%d, a2=%d data: %d, %d, %d, %d, %d ", _value,length,function,address1,address2,_data[6],_data[7],_data[8], _data[9], _data[10]);
        Log.info(status);
//    }
}

int MR24::situation_judgment(int ad1, int ad2, int ad3, int ad4, int ad5)
{
    if(ad1 == REPORT_RADAR || ad1 == REPORT_OTHER){ // 0x03, 0x05
        if(ad2 == ENVIRONMENT || ad2 == HEARTBEAT){ // 0x05, 0x01
            if(ad3 == NOBODY){                      // 0x00
                _statusMessage = "Couch nobody";
                return DETECTED_NOBODY;             // 0
            }
            else if(ad3 == SOMEBODY_BE && ad4 == SOMEBODY_MOVE){
                _statusMessage = "Couch movement";
                return DETECTED_MOVEMENT;           // 100
            }
            else if(ad3 == SOMEBODY_BE && ad4 == SOMEBODY_STOP){
                _statusMessage = "Couch occupied stop";
                return DETECTED_SOMEBODY;           // 50
            }
        }
        else if(ad2 == CLOSE_AWAY){
            if(ad3 == CA_BE && ad4 == CA_BE){
                if(ad5 == CA_BE){
                    _statusMessage = "Couch occupied";
                    return DETECTED_SOMEBODY;       // 50
                }
                else if(ad5 == CA_CLOSE){
                    _statusMessage = "Couch occupied close";
                    return DETECTED_SOMEBODY_CLOSE; // 75
                }
                else if(ad5 == CA_AWAY){
                    _statusMessage = "Couch occupied away";
                    return DETECTED_SOMEBODY_FAR;   // 25
                }
            }
        }
    }
    _statusMessage = "Couch nothing";
    return DETECTED_NOTHING;
}

void MR24::sendScene(int scene) {
    int data[10] = { 0x55, 10, 0, 2, 4, 16, 0, 0, 0 };
    
    data[6] = scene;
    int crc = calcCRC(data);
    data[7] = crc & 0xff;
    data[8] = (crc << 8) & 0xff;
}

void MR24::sendSensitivity(int sensitivity) {
    
}

int MR24::calcCRC(int *data, int length) {
    int crc = 0;
    //TODO: refer to doc Appendex 1
    return crc;
}

/**
 * notify()
 * Publish switch state
 */
void MR24::notify()
{
    logMessage();
    Log.info("Radar change from "+_prevStatusMessage+" to "+_statusMessage);
    _prevStatusMessage = _statusMe ssage;
    
    String topic = "patriot/" + _name;
    String message = String(_value);
     IoT::mqttPublish(topic,message);
}

const unsigned char cuc_CRCHi[25 6]=
{
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40
};
