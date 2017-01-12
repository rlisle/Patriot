/*
 * LisleRV Common definitions
 */

#ifndef __LISLERV_H__
#define __LISLERV_H__

#define NumberOfMillisInSecond  1000
#define DebounceDelay           50
#define kDefaultAliveInterval   60000 //900000 // 15 minutes (1 minute for testing)
#define kPublishName            "lislerv"

// IoTv2 board I/O definitions
#define kLed1Pin         A3   // pin 9
#define kLed2Pin         RX   // pin 4
#define kLed3Pin         TX   // pin 3
#define kLed4Pin         WKP  // pin 5
#define k3vIo1Pin        DAC  // pin 6
#define k3vIo2Pin        A5   // pin 7
#define kPhotoPin        A4   // pin 8
#define k5vIo1Pin        A2   // pin 10
#define k5vIo2Pin        A1   // pin 11
#define k5vIo3Pin        A0   // pin 12
#define kStepper1Pin1    D0   // pin 13
#define kStepper1Pin2    D1   // pin 14
#define kStepper1Pin3    D2   // pin 15
#define kStepper1Pin4    D3   // pin 16
#define kStepper2Pin1    D4   // pin 17
#define kStepper2Pin2    D5   // pin 18
#define kStepper2Pin3    D6   // pin 19
#define kStepper2Pin4    D7   // pin 20

// Devices
#define kBedLeftSpots      "bedleft"
#define kBedRightSpots     "bedright"
#define kBedCenterLights   "bedcenter"
#define kBoothLamp         "boothlamp"
#define kBoothSpots        "boothspots"
#define kBoothSwitch       "boothswitch"
#define kCouchLeftSpots    "couchleft"
#define kCouchRightSpots   "couchright"
#define kCounterSpots      "counterspots"
#define kMainLights        "mainlights"
#define kMicrowaveSpots    "microwavespots"
#define kMicrowaveCabinet  "microwavecabinet"
#define kOutsideLights     "outsidelights"
#define kOverheadLights    "overheadlights"
#define kPianoSpots        "pianospots"
#define kRampLights        "ramplights"
#define kRearOverheadLights "rearoverhead"
#define kShowerFan         "showerfan"
#define kShowerLight       "showerlight"

#define kActivitiesVariableName  "Activities"
#define kControllersVariableName "Controllers"
#define kDevicesVariableName     "Devices"
#define kMaxVariableStringLength 266

// Activities
#define kAllLightsActivity  "alllights"
#define kAwayActivity       "away"
#define kBedLightsActivity  "bed"
#define kBedLeftReading     "ronsbed"
#define kBedRightReading    "shelleysbed"
#define kBoothActivity      "booth"
#define kCoffeeActivity     "coffee"
#define kComputerActivity   "computer"
#define kCookActivity       "cook"
#define kDishesActivity     "dishes"
#define kHomeActivity       "home"
#define kKitchenActivity    "kitchen"
#define kMovieActivity      "movie"
#define kOverheadActivity   "overhead"
#define kOutsideActivity    "outside"
#define kPianoActivity      "piano"
#define kRampActivity       "ramp"
#define kCouchRightActivity "ronscouch"
#define kCouchLeftActivity  "shelleyscouch"
#define kShowerActivity     "shower"
#define kTVActivity         "tv"

#endif // __LISLERV_H__
