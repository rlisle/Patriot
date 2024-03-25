// LeftSlide Settings

#define LIVINGROOM_MOTION_TIMEOUT_MSECS 15*1000

//TODO: change to const
#define CONTROLLER_NAME "LeftSlide"
#define MQTT_BROKER "192.168.0.33"

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(AUTOMATIC);

//#define PIR_POLL_INTERVAL_MILLIS 500
int const livingRoomMotionTimeoutSecs = 3*60;


// Write Log messages to MQTT and/or serial port
#define MQTT_LOGGING true
// View logs with CLI using 'particle serial monitor --follow'
//SerialLogHandler logHandler1(57600, LOG_LEVEL_ALL);

// Timing
bool isTimingLivingRoomMotion;

//bool couchPresenceFiltered = 0;
//long lastCouchPresence = 0;
//int couchPresence = 0;

void setupWifi() {
//    WiFi.setCredentials(WIFI_SSID, WIFI_PASSWORD);
//    WiFi.selectAntenna(ANT_INTERNAL);    
//    WiFi.useDynamicIP();
}