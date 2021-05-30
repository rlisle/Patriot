//
//  ParticleNetwork.m
//  ParticleSDKPods
//
//  Created by Ido Kleinman on 9/24/18.
//  Copyright © 2018 Particle Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ParticleNetwork.h"
#import "ParticleErrorHelper.h"
#import "ParticleCloud.h"
#import "ParticleLogger.h"
#import <objc/runtime.h>


#ifdef USE_FRAMEWORKS
#import <AFNetworking/AFNetworking.h>
#else
#import "AFHTTPSessionManager.h"
#endif

@interface ParticleNetwork()
@property (nonatomic, strong) AFHTTPSessionManager *manager;
@property (nonatomic, strong) NSURL *baseURL;
@end


@implementation ParticleNetwork

-(nullable instancetype)initWithParams:(NSDictionary *)params
{
    if (self = [super init])
    {
        _baseURL = [NSURL URLWithString:ParticleCloud.sharedInstance.currentBaseURL];
        if (!_baseURL) {
            return nil;
        }
        
        _name = nil;
        if ([params[@"name"] isKindOfClass:[NSString class]])
        {
            _name = params[@"name"];
        }
        
        _id = nil;
        if ([params[@"id"] isKindOfClass:[NSString class]])
        {
            _id = params[@"id"];
        }

        _panId = nil;
        if ([params[@"pan_id"] isKindOfClass:[NSString class]])
        {
            _panId = params[@"pan_id"];
        }

        _xpanId = nil;
        if ([params[@"xpan_id"] isKindOfClass:[NSString class]])
        {
            _xpanId = params[@"xpan_id"];
        }
        
        _notes = nil;
        if ([params[@"notes"] isKindOfClass:[NSString class]])
        {
            _notes = params[@"notes"];
        }
        
        if ([params[@"type"] isKindOfClass:[NSString class]])
        {
            if ([params[@"type"] isEqualToString:@"micro_wifi"]) {
                _type = ParticleNetworkTypeMicroWifi;
            } else if ([params[@"type"] isEqualToString:@"micro_cellular"]) {
                _type = ParticleNetworkTypeMicroCellular;
            } else if ([params[@"type"] isEqualToString:@"high_availability"]) {
                _type = ParticleNetworkTypeHighAvailability;
            } else if ([params[@"type"] isEqualToString:@"large_site"]) {
                _type = ParticleNetworkTypeLargeSite;
            } else {
                _type = ParticleNetworkTypeMicroWifi;
            }

        }
        
        _gatewayCount = 0;
        if ([params[@"gateway_count"] isKindOfClass:[NSNumber class]])
        {
            _gatewayCount = [params[@"gateway_count"] intValue];
        }
        
        _deviceCount = 0;
        if ([params[@"device_count"] isKindOfClass:[NSNumber class]])
        {
            _deviceCount = [params[@"device_count"] intValue];
        }
        
        _channel = 0;
        if ([params[@"channel"] isKindOfClass:[NSNumber class]])
        {
            _channel = [params[@"channel"] intValue];
        }
        
        if ([params[@"state"] isKindOfClass:[NSString class]])
        {
            if ([params[@"state"] isEqualToString:@"pending"]) {
                _state = ParticleNetworkStatePending;
            } else {
                _state = ParticleNetworkStateConfirmed;
            }
        } else { // this is at least till API returns the state for all network calls - then we can remove it and keep last state
            _state = ParticleNetworkStateConfirmed;
        }
        
        
        if ([params[@"last_heard"] isKindOfClass:[NSString class]])
        {
            // TODO: add to utils class as POSIX time to NSDate
            NSString *dateString = params[@"last_heard"];// "2015-04-18T08:42:22.127Z"
            NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
            [formatter setDateFormat:@"yyyy-MM-dd'T'HH:mm:ss.SSSZ"];
            NSLocale *posix = [[NSLocale alloc] initWithLocaleIdentifier:@"en_US_POSIX"];
            [formatter setLocale:posix];
            _lastHeard = [formatter dateFromString:dateString];
        }

        self.manager = [[AFHTTPSessionManager alloc] initWithBaseURL:self.baseURL];
        self.manager.responseSerializer = [AFJSONResponseSerializer serializer];
        [self.manager.requestSerializer setValue:@"ios-cloud-sdk" forHTTPHeaderField:@"X-Particle-Tool"];

        [ParticleLogger logDebug:@"ParticleNetwork" format:@"self = %@", self];

        if (!self.manager) return nil;
        
        return self;
    }
    
    return nil;
}

-(NSString *)description
{
    NSString *desc = [NSString stringWithFormat:@"<ParticleNetwork 0x%lx id: %@, name: %@, type: %d, panId: %@, xpanId: %@, channel: %u, deviceCount: %u, gatewayCount: %u, notes: %@, state: %d, lastHeard: %@>",
                                                (unsigned long)self,
                                                self.id,
                                                self.name,
                                                self.type,
                                                self.panId,
                                                self.xpanId,
                                                self.channel,
                                                self.deviceCount,
                                                self.gatewayCount,
                                                self.notes,
                                                self.state,
                                                self.lastHeard];

    return desc;

}

-(NSURLSessionDataTask *)refresh:(nullable ParticleCompletionBlock)completion
{
    return [[ParticleCloud sharedInstance] getNetwork:self.id completion:^(ParticleNetwork * _Nullable updatedNetwork, NSError * _Nullable error) {
        if (!error)
        {
            if (updatedNetwork)
            {
                // if we got an updated network from the cloud - overwrite ALL self's properies with the new device properties (except for delegate which should be copied over)
                NSMutableSet *propNames = [NSMutableSet set];
                unsigned int outCount, i;
                objc_property_t *properties = class_copyPropertyList([updatedNetwork class], &outCount);
                for (i = 0; i < outCount; i++) {
                    objc_property_t property = properties[i];
                    NSString *propertyName = [[NSString alloc] initWithCString:property_getName(property) encoding:NSStringEncodingConversionAllowLossy];
                    [propNames addObject:propertyName];
                }
                free(properties);

//                if (self.delegate) {
//                    updatedDevice.delegate = self.delegate;
//                }

                for (NSString *property in propNames)
                {
                    id value = [updatedNetwork valueForKey:property];
                    [self setValue:value forKey:property];
                }
            }
            if (completion)
            {
                completion(nil);
            }
        }
        else
        {
            if (completion)
            {
                completion(error);
            }
        }
    }];

    return nil;
}







@end
