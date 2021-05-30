//
// Created by Raimundas Sakalauskas on 10/07/2018.
// Copyright (c) 2018 Ido Kleinman. All rights reserved.
//

#import "ParticleErrorHelper.h"

#ifdef USE_FRAMEWORKS
#import <AFNetworking/AFNetworking.h>
#else
#import "AFHTTPSessionManager.h"
#endif

NSString * const ParticleSDKErrorResponseBodyKey = @"io.particle.error.response.body";
NSString * const ParticleSDKErrorLocalizedStringKey = @"io.particle.error.localizedstring";

@implementation ParticleErrorHelper

+ (NSError *)getParticleError:(NSError *)networkError task:(NSURLSessionDataTask *_Nullable)task customMessage:(NSString *)message {

    int code = -1;
    if (task) {
        NSHTTPURLResponse *serverResponse = (NSHTTPURLResponse *)task.response;
        code = (int)serverResponse.statusCode;
    }

    NSMutableDictionary *errorDetail = [NSMutableDictionary dictionary];

    if (networkError) {
        NSData *errorData = networkError.userInfo[AFNetworkingOperationFailingURLResponseDataErrorKey];
        if (errorData) {
            NSDictionary *serializedFailedBody = [NSJSONSerialization JSONObjectWithData:errorData options:kNilOptions error:nil];
            [errorDetail setValue:serializedFailedBody forKey:ParticleSDKErrorResponseBodyKey];
            [errorDetail setValue:[ParticleErrorHelper getErrorMessage:serializedFailedBody] forKey:ParticleSDKErrorLocalizedStringKey];
        }
    }

    if (message) {
        [errorDetail setValue:message forKey:NSLocalizedDescriptionKey];
    } else if (errorDetail[ParticleSDKErrorLocalizedStringKey]) {
        [errorDetail setValue: errorDetail[ParticleSDKErrorLocalizedStringKey] forKey:NSLocalizedDescriptionKey];
    } else {
        [errorDetail setValue: @"Unknown error occurred." forKey:NSLocalizedDescriptionKey];
    }

    return [NSError errorWithDomain:@"ParticleAPIError" code:code userInfo:errorDetail];
}

+ (NSString *)getErrorMessage:(NSDictionary *)response {
    if (response[@"error_description"]) {
        //error_description should be prioritized against error
        //{ error: 'error1', error_description: 'error2' }
        return response[@"error_description"];
    } else if (response[@"error"]) {
        //  { error: 'error1' }
        return response[@"error"];
    } else if (response[@"err"]){
            //  { err: 'error1' }
            return response[@"err"];
    } else if (response[@"errors"]) {
        if ([response[@"errors"] isKindOfClass:[NSArray class]]) {
            NSArray *errors = (NSArray *)response[@"errors"];
            NSMutableString *outputMessage = [[NSMutableString alloc] init];
            for (int i = 0; i < errors.count; i++) {
                if ([errors[i] isKindOfClass:[NSString class]]) {
                    //    { errors: ['error1', 'error2'] }
                    if (outputMessage.length > 0){
                        [outputMessage appendString:@"\r\n"];
                    }
                    [outputMessage appendString:errors[i]];
                } else if ([errors[i] isKindOfClass:[NSDictionary class]]) {
                    if (errors[i][@"error"]) {
                        if ([errors[i][@"error"] isKindOfClass:[NSString class]]) {
                            //  { errors: [ { error: 'error1' }, { error: 'error2' } ] }
                            if (outputMessage.length > 0) {
                                [outputMessage appendString:@"\r\n"];
                            }
                            [outputMessage appendString:errors[i][@"error"]];
                        } else if ([errors[i][@"error"] isKindOfClass:[NSDictionary class]] && errors[i][@"error"][@"status"] && [errors[i][@"error"][@"status"] isKindOfClass:[NSString class]]) {
                            //  { errors: [ { error: { status: 'error1' } }, { error: { status: 'error2' } } ] }
                            if (outputMessage.length > 0) {
                                [outputMessage appendString:@"\r\n"];
                            }
                            [outputMessage appendString:errors[i][@"error"][@"status"]];
                        } else {
                            // Unable to parse
                            return @"Unknown error occurred.";
                        }
                    } else {
                        // Unable to parse
                        return @"Unknown error occurred.";
                    }
                } else {
                    // Unable to parse
                    return @"Unknown error occurred.";
                }
            }
            return outputMessage;
        } else {
            // Unable to parse
            return @"Unknown error occurred.";
        }
    } else if (response[@"info"]) {
        //  { info: 'error1' }
        return response[@"info"];
    } else {
        //  { }
        return @"Unknown error occurred.";
    }
}

@end
