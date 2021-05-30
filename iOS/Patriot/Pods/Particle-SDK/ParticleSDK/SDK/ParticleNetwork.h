//
//  ParticleNetwork.h
//  ParticleSDKPods
//
//  Created by Ido Kleinman on 9/24/18.
//  Copyright © 2018 Particle Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ParticleDevice.h"

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, ParticleNetworkType) {
    ParticleNetworkTypeMicroWifi=0,
    ParticleNetworkTypeMicroCellular,
    ParticleNetworkTypeHighAvailability,
    ParticleNetworkTypeLargeSite
};


 // for now API will only return confirmed networks
typedef NS_ENUM(NSInteger, ParticleNetworkState) {
    ParticleNetworkStatePending=0,
    ParticleNetworkStateConfirmed
};

@interface ParticleNetwork : NSObject


extern NSString *const kParticleAPIBaseURL;

/*!
 * Unique network identifier.
 */
@property (nonatomic, strong, readonly) NSString* id;

/*!
 * Network name.Unique for the account.
 */
@property (nonatomic, strong, readonly) NSString* name;

/*!
 * Type of the network. Wifi, cellular or high availability.
 */
@property (nonatomic, readonly) ParticleNetworkType type;

/*!
 * Network Pan ID
 */
@property (nonatomic, strong, nullable, readonly) NSString* panId;

/*!
 * Network XPan ID
 */
@property (nonatomic, strong, nullable, readonly) NSString* xpanId;

/*!
 * Network channel.
 */
@property (nonatomic, readonly) NSUInteger channel;

/*!
 * Number of devices within the network.
 */
@property (nonatomic, readonly) NSUInteger deviceCount;

/*!
 * Number of gateways within the network.
 */
@property (nonatomic, readonly) NSUInteger gatewayCount;

/*!
 * Last time any of the network devices were noticed online.
 */
@property (nonatomic, strong, nullable, readonly) NSDate* lastHeard;

/*!
 * Additional notes about the network.
 */
@property (nonatomic, nullable) NSString* notes;

/*!
 * Network state between pending and confirmed. Network becomes confirmed when it's gateway comes online.
 */
@property (nonatomic, readonly) ParticleNetworkState state;


-(nullable instancetype)initWithParams:(NSDictionary *)params NS_DESIGNATED_INITIALIZER;
-(instancetype)init __attribute__((unavailable("Must use initWithParams:")));

/**
 *  Request network refresh from cloud.
 *
 *  @param completion Completion block called when function completes with NSError object in case of an error or nil if success.
 *
 */
-(NSURLSessionDataTask *)refresh:(nullable ParticleCompletionBlock)completion;


@end

NS_ASSUME_NONNULL_END
