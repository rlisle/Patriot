//
// Created by Raimundas Sakalauskas on 2019-04-05.
// Copyright (c) 2019 Particle Inc. All rights reserved.
//

#import "ParticleSimInfo.h"
#import "ParticleLogger.h"


@implementation ParticleSimInfo

-(instancetype)initWithParams:(NSDictionary *)params
{
    if (self = [super init])
    {
        _mbLimit = [params[@"mb_limit"] intValue];
        if ([params[@"status"] isKindOfClass:[NSString class]])
        {
            NSString * status = params[@"status"];

            if ([status.lowercaseString isEqualToString:@"active"]) {
                _status = ParticleSimDetailedStatusActivate;
            } else if ([status.lowercaseString isEqualToString:@"never_before_activated"]) {
                _status = ParticleSimDetailedStatusInactiveNeverActivated;
            } else if ([status.lowercaseString isEqualToString:@"inactive_user_deactivated"]) {
                _status = ParticleSimDetailedStatusInactivateUserDeactivated;
            } else if ([status.lowercaseString isEqualToString:@"inactive_data_limit_reached"]) {
                _status = ParticleSimDetailedStatusInactiveDataLimitReached;
            } else if ([status.lowercaseString isEqualToString:@"inactive_invalid_payment_method"]) {
                _status = ParticleSimDetailedStatusInactiveInvalidPayment;
            }
        }

        [ParticleLogger logDebug:@"ParticleSimInfo" format:@"self = %@", self];
        return self;

    }

    return nil;
}

- (NSString *)description {
    NSMutableString *description = [NSMutableString stringWithFormat:@"<%@: ", NSStringFromClass([self class])];
    [description appendFormat:@"mbLimit: %i ", _mbLimit];
    [description appendFormat:@"status: %d ", _status];
    [description appendString:@">"];
    return description;
}


@end