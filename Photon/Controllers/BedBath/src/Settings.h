// BedBath Settings

// Generally uncomment only 1 of the following 2 logs
#define MQTT_LOGGING false
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler1(57600, LOG_LEVEL_INFO);

//TODO: change to const
#define CONTROLLER_NAME "BedBath"
#define MQTT_BROKER "192.168.0.33"

//TODO: move timeouts into IoT
#define TESTDOOR_TIMEOUT_MSECS 10*1000

//#define PCA9634_ADDRESS 1       // 1st jumper
//#define I2CR4IO4_ADDRESS 0x20  // 4xRelay+4GPIO address (0x20 = no jumpers)
