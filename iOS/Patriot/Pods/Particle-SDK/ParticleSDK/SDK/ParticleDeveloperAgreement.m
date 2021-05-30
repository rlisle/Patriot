//
//  ParticleDeveloperAgreement.m
//
//  Created by Raimundas Sakalauskas on 1/2/21.
//  Copyright © 2021 Particle Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ParticleDeveloperAgreement.h"
#import "ParticleLogger.h"

@implementation ParticleDeveloperAgreement

-(instancetype)initWithParams:(NSDictionary *)params
{
    if (self = [super init])
    {
        NSLog(@"params = %@", params);

        NSDictionary *attributes = params[@"attributes"];
        if (attributes != nil) {
            if ([[attributes[@"agreement_type"] stringValue] isEqualToString:@"developer"]) {
                _deviceLimitReached = NO;
                if ([attributes[@"current_usage_summary"][@"device_limit_reached"] isKindOfClass:[NSNumber class]]) {
                    _deviceLimitReached = [attributes[@"current_usage_summary"][@"device_limit_reached"] boolValue];
                }

                _maxDevices = 0;
                if ([attributes[@"pricing_terms"][@"device"][@"max_devices"] isKindOfClass:[NSNumber class]]) {
                    _maxDevices = [attributes[@"pricing_terms"][@"device"][@"max_devices"] intValue];
                }

                return self;
            }
        }
    }

    return nil;
}

- (NSString *)description {
    NSMutableString *description = [NSMutableString stringWithFormat:@"<%@: ", NSStringFromClass([self class])];
    [description appendFormat:@"_maxDevices: %i ", _maxDevices];
    [description appendFormat:@"_deviceLimitReached: %i ", _deviceLimitReached];
    [description appendString:@">"];
    return description;
}

@end
