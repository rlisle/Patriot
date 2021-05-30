//
//  ParticleDeveloperAgreement.h
//
//  Created by Raimundas Sakalauskas on 1/2/21.
//  Copyright © 2021 Particle Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ParticleDevice.h"

NS_ASSUME_NONNULL_BEGIN

@interface ParticleDeveloperAgreement : NSObject

/*!
 * Number of max devices allowed by the current agreement.
 */
@property (nonatomic) int maxDevices;

/*!
 * This flag is true if plan device quota is exceeded.
 */
@property (nonatomic) BOOL deviceLimitReached;

- (instancetype)initWithParams:(NSDictionary *)params;

@end

NS_ASSUME_NONNULL_END
