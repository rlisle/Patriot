/******************************************************************
PatriotMR24 plugin

Features:
- Use Rx/Tx if pins specified as 0,0

http://www.github.com/rlisle/Patriot

This is a Patriot plugin. After making changes use "particle library upload", etc.
 
 History
   4/17/22 Was toggling between 50/100, so removed 50 level,
         Add timer before turn-off to prevent frequent 0/100 toggling
 
Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

 Messages:
 0x55, LLo, LHi, Fn, A1, A2, Data, CrcL, CrcH
 
******************************************************************/

#include "PatriotMR24.h"
#include "IoT.h"

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
    _lastMotion = 0;
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
    int oldValue = _value;
    _s1value = digitalRead(_s1pin);
    _s2value = digitalRead(_s2pin);
    int newValue = (_s1value || _s2value) ? 100 : 0;
    if(newValue != oldValue) {
        if(newValue == 100) {
            _lastMotion = millis();
            _value = 100;       // Do we need a filter for turn-on?
        } else {
            if(millis() > _lastMotion + TURNOFF_DELAY) {
                _value = 0;
            }
        }
    }
    return (oldValue != _value);
}

/**
 Receive any data.
 If complete message received:
 * Set value
 * Return true if value changed
 _index can be used to determine if any data was received
 */
bool MR24::didTxRxSensorChange()
{
    int Msg;
    int prevValue = _value;
    int newValue = prevValue;

    while(Serial1.available()) {
        Msg = Serial1.read();
        if(Msg == MESSAGE_HEAD && _index > 0){      // Skip first time
            _index = 0;                             // Loop back to start of buffer
            parseMessage();
            int newValue = situation_judgment();    //was passing _data[5], _data[6], _data[7], _data[8], _data[9]
        }
        if(_index < 14) {   // Safety check. Should not happen.
            _data[_index++] = Msg;
        }
    }
    if(newValue != prevValue) {
        if(newValue == 100) {
            _lastMotion = millis();
            _value = 100;       // Do we need a filter for turn-on?
        } else {
            if(millis() > _lastMotion + TURNOFF_DELAY) {
                _value = 0;
            }
        }
    }

    return _value != prevValue;
}

void MR24::parseMessage() {
    _length = _data[1] + (_data[2] << 8);
    _function = _data[3];
    _address1 = _data[4];
    _address2 = _data[5];
    _d1 = _data[6];
    _d2 = _data[7];
    _d3 = _length > 10 ? _data[8] : 0;
    _crc = _data[_length-2] | (_data[_length-1] << 8);
}


void MR24::logMessage() {
    if(_length == 10) {
        String status10 = String::format("Radar %d, L:%d, F:%d, A1:%d, A2:%d D: %d, %d, crc: %x", _value,_length,_function,_address1,_address2,_data[6],_data[7],_crc);
        Log.info(status10);
    } else if(_length == 11) {
        String status11 = String::format("Radar %d, L:%d, F:%d, A1:%d, A2:%d D: %d, %d, %d, crc: %x", _value,_length,_function,_address1,_address2,_data[6],_data[7],_data[8],_crc);
        Log.info(status11);
    } else {
        Log.info("Radar: length != 10 or 11");
    }
}

int MR24::situation_judgment()
{
    if(_address1 == REPORT_RADAR || _address1 == REPORT_OTHER){ // 0x03, 0x05
        if(_address2 == ENVIRONMENT || _address2 == HEARTBEAT){ // 0x05, 0x01
            if(_d1 == NOBODY){                      // 0x00
                _statusMessage = "Couch nobody";
                return 0; // was DETECTED_NOBODY;             // 0
            }
            else if(_d1 == SOMEBODY_BE && _d2 == SOMEBODY_MOVE){
                _statusMessage = "Couch movement";
                return 100; // was DETECTED_MOVEMENT;           // 100
            }
            else if(_d1 == SOMEBODY_BE && _d2 == SOMEBODY_STOP){
                _statusMessage = "Couch occupied stop";
                return 100; // was DETECTED_SOMEBODY;           // 50
            }
        }
        else if(_address2 == CLOSE_AWAY){
            if(_d1 == CA_BE && _d2 == CA_BE){
                if(_d3 == CA_BE){
                    _statusMessage = "Couch occupied";
                    return 100; // was DETECTED_SOMEBODY;       // 50
                }
                else if(_d3 == CA_CLOSE){
                    _statusMessage = "Couch occupied close";
                    return 100; // was DETECTED_SOMEBODY_CLOSE; // 75
                }
                else if(_d3 == CA_AWAY){
                    _statusMessage = "Couch occupied away";
                    return 100; // was DETECTED_SOMEBODY_FAR;   // 25
                }
            }
        }
    }
    _statusMessage = "Couch nothing";
    return DETECTED_NOTHING;
}

void MR24::sendScene(int scene) {
    unsigned char data[9] = { 0x55, 9, 0, 2, 4, 16, 0, 0, 0 };
    
    data[6] = scene;
    int crc = calcCRC(&data[1],6);
    data[7] = crc & 0xff;
    data[8] = (crc << 8) & 0xff;
    Serial1.write(data,9);
}

void MR24::sendSensitivity(int sensitivity) {
    unsigned char data[9] = { 0x55, 9, 0, 2, 4, 16, 0, 0, 0 };
    
    data[6] = sensitivity;
    int crc = calcCRC(&data[1],6);
    data[7] = crc & 0xff;
    data[8] = (crc << 8) & 0xff;
    Serial1.write(data,9);
}

/**
 * notify()
 * Publish switch state
 */
void MR24::notify()
{
    logMessage();
    Log.info("Radar change from "+_prevStatusMessage+" to "+_statusMessage);
    _prevStatusMessage = _statusMessage;
    
    String message = String(_value);
     IoT::publishMQTT(_name,message);
}

const unsigned char crcHiTable[256] =
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

const unsigned char crcLoTable[256] =
{
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
0x41, 0x81, 0x80, 0x40
};


int MR24::calcCRC(unsigned char *data, int length) {
    unsigned char crcHi = 0xff;
    unsigned char crcLo = 0xff;
    int index = 0;
    
    while(length--) {
        index = crcLo ^ *(data++);
        crcLo = (unsigned char)(crcHi ^ crcHiTable[index]);
        crcHi = crcLoTable[index];
    }
    return crcLo | (crcHi << 8);    // Example does this the opposite
}
