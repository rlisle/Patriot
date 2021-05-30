//
// Created by Raimundas Sakalauskas on 10/07/2018.
// Copyright (c) 2018 Ido Kleinman. All rights reserved.
//

#import <Foundation/Foundation.h>

//Serialized response JSON
FOUNDATION_EXPORT NSString * const ParticleSDKErrorResponseBodyKey;
FOUNDATION_EXPORT NSString * const ParticleSDKErrorLocalizedStringKey;

@interface ParticleErrorHelper : NSObject

/// Helper method used while creating standard Particle SDK error. Returning NSError will have ParticleSDKErrorResponseBodyKey,
/// ParticleSDKErrorLocalizedStringKey and localizedDescription set.
///
/// @param networkError error object returned by AFNetworking.
/// @param task NSURLSessionDataTask that resulted in an error.
/// @param message Custom error message that should be used to override default error message.
+ (NSError *)getParticleError:(nullable NSError *)networkError task:(nullable NSURLSessionDataTask *)task customMessage:(nullable NSString *)message;


/// Get the best possible error message from error response body.
///
/// @param response Error response body.
+ (NSString *)getErrorMessage:(nonnull NSDictionary *)response;

@end
