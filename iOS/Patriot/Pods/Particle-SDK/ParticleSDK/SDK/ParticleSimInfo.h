//
// Created by Raimundas Sakalauskas on 2019-04-05.
// Copyright (c) 2019 Particle Inc. All rights reserved.
//

#import <Foundation/Foundation.h>


typedef NS_ENUM(NSInteger, ParticleSimDetailedStatus) {
    ParticleSimDetailedStatusActivate = 0,
    ParticleSimDetailedStatusInactiveNeverActivated,
    ParticleSimDetailedStatusInactivateUserDeactivated,
    ParticleSimDetailedStatusInactiveDataLimitReached,
    ParticleSimDetailedStatusInactiveInvalidPayment
};

@interface ParticleSimInfo : NSObject

/*!
 * Data limit set for SIM card.
 */
@property (nonatomic, readonly) int mbLimit;

/*!
 * SIM card status (active, inactive, etc.)
 */
@property (nonatomic, readonly) ParticleSimDetailedStatus status;

- (instancetype)initWithParams:(NSDictionary *)params;

@end

