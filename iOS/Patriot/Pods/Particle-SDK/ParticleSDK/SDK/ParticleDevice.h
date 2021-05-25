//
//  ParticleDevice.h
//  Particle iOS Cloud SDK
//
//  Created by Ido Kleinman
//  Copyright 2015 Particle
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#import <Foundation/Foundation.h>
#import "ParticleEvent.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * Standard completion block for API calls, will be called when the task is completed
 * with a nullable error object that will be nil if the task was successful.
 */
typedef void (^ParticleCompletionBlock)(NSError * _Nullable error);

typedef NS_ENUM(NSInteger, ParticleDeviceType) {
    ParticleDeviceTypeUnknown=-1,
    ParticleDeviceTypeCore=0,
    ParticleDeviceTypePhoton=6, // or P0
    ParticleDeviceTypeP1=8,
    ParticleDeviceTypeElectron=10, // or E0
    ParticleDeviceTypeRaspberryPi=31,
    ParticleDeviceTypeRedBearDuo=88,
    ParticleDeviceTypeBluz=103,
    ParticleDeviceTypeDigistumpOak=82,
    ParticleDeviceTypeESP32=11,
    ParticleDeviceTypeArgon=12,
    ParticleDeviceTypeBoron=13,
    ParticleDeviceTypeXenon=14,
    ParticleDeviceTypeASeries=22,
    ParticleDeviceTypeBSeries=23,
    ParticleDeviceTypeXSeries=24,
    ParticleDeviceTypeB5SoM=25
};

typedef NS_ENUM(NSInteger, ParticleDeviceSystemEvent) {
    ParticleDeviceSystemEventCameOnline,
    ParticleDeviceSystemEventWentOffline,
    ParticleDeviceSystemEventFlashStarted,
    ParticleDeviceSystemEventFlashSucceeded,
    ParticleDeviceSystemEventFlashFailed,
    ParticleDeviceSystemEventAppHashUpdated,
    ParticleDeviceSystemEventEnteredSafeMode,
    ParticleDeviceSystemEventSafeModeUpdater
};

typedef NS_ENUM(NSInteger, ParticleDeviceNetworkRole) {
    ParticleDeviceNetworkRoleGateway,
    ParticleDeviceNetworkRoleNode,
};

typedef NS_ENUM(NSInteger, ParticleDeviceNetworkRoleState) {
    ParticleDeviceNetworkRoleStatePending,
    ParticleDeviceNetworkRoleStatePendingConfirmed,
    ParticleDeviceNetworkRoleStatePendingOwnerApproval
};

@class ParticleDevice;

@protocol ParticleDeviceDelegate <NSObject>

@optional
-(void)particleDevice:(ParticleDevice *)device didReceiveSystemEvent:(ParticleDeviceSystemEvent)event;

@end

@interface ParticleDevice : NSObject

/**
 *  DeviceID string
 */
@property (strong, nonatomic, readonly) NSString* id;

/**
 *  Device name. Device can be renamed in the cloud by setting this property. If renaming fails name will stay the same.
 */
@property (strong, nullable, nonatomic) NSString* name;

/**
 *  Is device connected to the cloud? Best effort - May not accurate reflect true state.
 */
@property (nonatomic, readonly) BOOL connected;

/**
 *  List of function names exposed by device. Functions are only available after `refresh` is called on the device instance returned by `getDevices` call.
 */
@property (strong, nonatomic, nonnull, readonly) NSArray<NSString *> *functions;

/**
 *  Dictionary of exposed variables on device with their respective types. Variables are only available after `refresh` is called on the device instance returned by `getDevices` call.
 */
@property (strong, nonatomic, nonnull, readonly) NSDictionary<NSString *, NSString *> *variables; // @{varName : varType, ...}

@property (strong, nonatomic, nullable, readonly) NSString *lastApp; __deprecated_msg("This property is not updated");

/**
 * Date when the device was last heard.
 */
@property (strong, nonatomic, nullable, readonly) NSDate *lastHeard;

/**
 * App hash received from system event after flashing a new different user application. Please note that if you flashe same app, the app hash event won't trigger.
 */
@property (strong, nonatomic, nullable, readonly) NSString *appHash;

/**
 * Mesh network ID if device belongs to a mesh network. `nil` otherwise.
 */
@property (strong, nonatomic, nullable, readonly) NSString *networkId;

/**
 * Mesh network role if device belongs to a mesh network (gateway or a node).
 */
@property (nonatomic, readonly) ParticleDeviceNetworkRole networkRole;

/**
 * Mesh network state if device belongs to a mesh network. State can be pending if device is waiting for the role change confirmation.
 */
@property (nonatomic, readonly) ParticleDeviceNetworkRoleState networkRoleState;

/**
 * Is device currently being flashed? Best effort - May not accurate reflect true state.
 */
@property (nonatomic, readonly) BOOL isFlashing;

/**
 * Last know IP address of the device.
 */
@property (strong, nonatomic, nullable, readonly) NSString *lastIPAdress;

/**
 * Last known ICCID (for cellular devices only)
 */
@property (strong, nonatomic, nullable, readonly) NSString *lastIccid;

/**
 * Last know IMEI (for cellular devices only)
 */
@property (strong, nonatomic, nullable, readonly) NSString *imei;

/**
 * Device platfomr ID (device type).
 */
@property (nonatomic, readonly) NSUInteger platformId;

/**
 * ID of the product device belongs to.
 */
@property (nonatomic, readonly) NSUInteger productId;


@property (strong, nonatomic, nullable, readonly) NSString *status; __deprecated_msg("This property is not updated");


/**
 * Unique serial number of the device
 */
@property (strong, nonatomic, nullable, readonly) NSString *serialNumber;

/**
 * Mobile secret of the device. Only available for 3rd gen devices.
 */
@property (strong, nonatomic, nullable, readonly) NSString *mobileSecret;

/**
 * Additional device notes. User editable.
 */
@property (strong, nonatomic, nullable) NSString *notes;

/**
 *  Device firmware version string
 */
@property (strong, nonatomic, readonly) NSString *systemFirmwareVersion;

/**
 * Is this device a cellular device?
 */
@property (nonatomic, readonly) BOOL cellular;

/**
 * Does this device require software update?
 */
@property (nonatomic, readonly) BOOL requiresUpdate;

/**
 * Particle device type. A easier to use version of `platformId`.
 */
@property (nonatomic, readonly) ParticleDeviceType type;

/**
 * `type` casted into a human readable String.
 */
@property (nonatomic, readonly) NSString *typeString;



-(nullable instancetype)initWithParams:(NSDictionary *)params NS_DESIGNATED_INITIALIZER;
-(instancetype)init __attribute__((unavailable("Must use initWithParams:")));

@property (nonatomic, strong) id <ParticleDeviceDelegate> delegate;

/**
 *  Retrieve a variable value from the device
 *
 *  @param variableName Variable name
 *  @param completion   Completion block to be called when function completes with the variable value retrieved (as id/AnyObject) or NSError object in case on an error
 */
-(NSURLSessionDataTask *)getVariable:(NSString *)variableName completion:(nullable void(^)(id _Nullable result, NSError* _Nullable error))completion;

/**
 *  Call a function on the device
 *
 *  @param functionName Function name
 *  @param args         Array of arguments to pass to the function on the device. Arguments will be converted to string maximum length 63 chars.
 *  @param completion   Completion block will be called when function was invoked on device. First argument of block is the integer return value of the function, second is NSError object in case of an error invoking the function
 */
-(NSURLSessionDataTask *)callFunction:(NSString *)functionName
                        withArguments:(nullable NSArray *)args
                           completion:(nullable void (^)(NSNumber * _Nullable result, NSError * _Nullable error))completion;

/**
 *  Signal device
 *  Will make the onboard LED "shout rainbows" for easy physical identification of a device
 *
 *  @param enable - YES to start or NO to stop LED signal.
 *
 */
-(NSURLSessionDataTask *)signal:(BOOL)enable completion:(nullable ParticleCompletionBlock)completion;


/**
 *  This will ping a device, enabling you to see if your device is online or offline
 */
- (NSURLSessionDataTask *)ping:(nullable void (^)(BOOL result, NSError *_Nullable error))completion;

/**
 *  Request device refresh from cloud
 *  update online status/functions/variables/device name, etc
 *
 *  @param completion Completion block called when function completes with NSError object in case of an error or nil if success.
 *
 */
-(NSURLSessionDataTask *)refresh:(nullable ParticleCompletionBlock)completion;

/**
 *  Remove device from current logged in user account
 *
 *  @param completion Completion block called when function completes with NSError object in case of an error or nil if success.
 */
-(NSURLSessionDataTask *)unclaim:(nullable ParticleCompletionBlock)completion;


/**
 *  Rename device
 *
 *  @param newName      New device name
 *  @param completion   Completion block called when function completes with NSError object in case of an error or nil if success.
 */
-(NSURLSessionDataTask *)rename:(NSString *)newName completion:(nullable ParticleCompletionBlock)completion;

/**
 *  Edit device notes
 *
 *  @param notes        Notes to be associated with particular device
 *  @param completion   Completion block called when function completes with NSError object in case of an error or nil if success.
 */
-(NSURLSessionDataTask *)setNotes:(NSString *)notes completion:(nullable ParticleCompletionBlock)completion;

/**
 *  Retrieve current data usage report (For Electron only)
 *
 *  @param completion   Completion block to be called when function completes with the data used in current payment period in (float)MBs. All devices other than Electron will return an error with -1 value
 */
-(NSURLSessionDataTask *)getCurrentDataUsage:(nullable void(^)(float dataUsed, NSError* _Nullable error))completion;


/**
 *  Flash files to device
 *
 *  @param filesDict    files dictionary in the following format: @{@"filename.bin" : <NSData>, ...} - that is a NSString filename as key and NSData blob as value. More than one file can be flashed. Data is alway binary.
 *  @param completion   Completion block called when function completes with NSError object in case of an error or nil if success. NSError.localized descripion will contain a detailed error report in case of a
 */
-(nullable NSURLSessionDataTask *)flashFiles:(NSDictionary *)filesDict completion:(nullable ParticleCompletionBlock)completion; //@{@"<filename>" : NSData, ...}

/**
 *  Flash known firmware images to device
 *
 *  @param knownAppName    NSString of known app name. Currently @"tinker" is supported. 
 *  @param completion      Completion block called when function completes with NSError object in case of an error or nil if success. NSError.localized descripion will contain a detailed error report in case of a
 */
-(NSURLSessionDataTask *)flashKnownApp:(NSString *)knownAppName completion:(nullable ParticleCompletionBlock)completion; // knownAppName = @"tinker", @"blinky", ... see http://docs.

#pragma mark Events subsystem functions

/**
 *  Subscribe to events from this specific (claimed) device - both public and private.
 *
 *  @param eventNamePrefix  Filter only events that match name eventNamePrefix, for exact match pass whole string, if nil/empty string is passed any event will trigger eventHandler
 *  @param eventHandler     Event handler function that accepts the event payload dictionary and an NSError object in case of an error
 */
-(nullable id)subscribeToEventsWithPrefix:(nullable NSString *)eventNamePrefix handler:(nullable ParticleEventHandler)eventHandler;

/**
 *  Unsubscribe from event/events.
 *
 *  @param eventListenerID The eventListener registration unique ID returned by the subscribe method which you want to cancel
 */
-(void)unsubscribeFromEventWithID:(id)eventListenerID;

// Internal use
-(void)__receivedSystemEvent:(ParticleEvent *)event;

@end

NS_ASSUME_NONNULL_END
