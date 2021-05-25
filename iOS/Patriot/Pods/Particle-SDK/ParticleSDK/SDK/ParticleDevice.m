//
//  ParticleDevice.m
//  mobile-sdk-ios
//
//  Created by Ido Kleinman on 11/7/14.
//  Copyright (c) 2014-2015 Particle. All rights reserved.
//

#import "ParticleDevice.h"
#import "ParticleCloud.h"
#import "ParticleNetwork.h"
#import "ParticleErrorHelper.h"
#import "ParticleLogger.h"
#import <objc/runtime.h>

#ifdef USE_FRAMEWORKS
#import <AFNetworking/AFNetworking.h>
#else
#import "AFHTTPSessionManager.h"
#endif

#define MAX_PARTICLE_FUNCTION_ARG_LENGTH 622

NS_ASSUME_NONNULL_BEGIN

@interface ParticleDevice()

@property (strong, nonatomic, nonnull) NSString* id;
@property (nonatomic) BOOL connected; // might be impossible
@property (strong, nonatomic, nonnull) NSArray *functions;
@property (strong, nonatomic, nonnull) NSDictionary *variables;
@property (strong, nonatomic, nullable) NSString *systemFirmwareVersion;
//@property (nonatomic) ParticleDeviceType type;
@property (nonatomic) BOOL requiresUpdate;
@property (nonatomic) BOOL isFlashing;
@property (nonatomic, strong) NSURL *baseURL;

@property (strong, nonatomic, nullable) NSString *lastIPAdress;
@property (strong, nonatomic, nullable) NSString *lastIccid; // Electron only
@property (strong, nonatomic, nullable) NSString *imei; // Electron only
@property (nonatomic) NSUInteger platformId;
@property (nonatomic) NSUInteger productId;
@property (strong, nonatomic, nullable) NSString *status;
@property (strong, nonatomic, nullable) NSString *appHash;
@property (strong, nonatomic, nullable) NSString *typeString;
@end

@implementation ParticleDevice

+ (AFHTTPSessionManager*) manager
{
    static dispatch_once_t onceToken;
    static AFHTTPSessionManager *manager = nil;
    dispatch_once(&onceToken, ^{
        manager = [[AFHTTPSessionManager alloc] initWithBaseURL:[NSURL URLWithString:ParticleCloud.sharedInstance.currentBaseURL]];
        manager.responseSerializer = [AFJSONResponseSerializer serializer];
        [manager.requestSerializer setValue:@"ios-cloud-sdk" forHTTPHeaderField:@"X-Particle-Tool"];
    });

    return manager;
}

- (NSString *)typeString {
        switch (self.type) {
            case ParticleDeviceTypeUnknown: return @"Unknown";
            case ParticleDeviceTypeCore : return @"Core";
            case ParticleDeviceTypePhoton : return @"Photon";
            case ParticleDeviceTypeP1 : return @"P1";
            case ParticleDeviceTypeElectron : return @"Electron";
            case ParticleDeviceTypeRaspberryPi : return @"RaspberryPi";
            case ParticleDeviceTypeRedBearDuo : return @"RedBearDuo";
            case ParticleDeviceTypeBluz : return @"Bluz";
            case ParticleDeviceTypeDigistumpOak : return @"DigistumpOak";
            case ParticleDeviceTypeESP32 : return @"ESP32";
            case ParticleDeviceTypeArgon : return @"Argon";
            case ParticleDeviceTypeASeries : return @"Argon";
            case ParticleDeviceTypeBoron : return @"Boron";
            case ParticleDeviceTypeBSeries : return @"Boron";
            case ParticleDeviceTypeXenon : return @"Xenon";
            case ParticleDeviceTypeXSeries : return @"Xenon";
            case ParticleDeviceTypeB5SoM : return @"B5SoM";
            default: return @"Unknown";
    }
}

-(nullable instancetype)initWithParams:(NSDictionary *)params
{
    if (self = [super init])
    {
        _baseURL = [NSURL URLWithString:ParticleCloud.sharedInstance.currentBaseURL];
        if (!_baseURL) {
            return nil;
        }
     
        _requiresUpdate = NO;
        
        _name = nil;
        if ([params[@"name"] isKindOfClass:[NSString class]])
        {
            _name = params[@"name"];
        }
        
        _connected = [params[@"connected"] boolValue] == YES;
        _cellular = [params[@"cellular"] boolValue] == YES;
        
        _functions = params[@"functions"] ?: @[];
        _variables = params[@"variables"] ?: @{};
        
        if (![_functions isKindOfClass:[NSArray class]]) {
            self.functions = @[];
        }

        if (![_variables isKindOfClass:[NSDictionary class]]) {
            _variables = @{};
        }

        _id = params[@"id"];

        _type = ParticleDeviceTypeUnknown;
        if ([params[@"platform_id"] isKindOfClass:[NSNumber class]])
        {
            self.platformId = [params[@"platform_id"] intValue];

            switch (self.platformId) {
                case ParticleDeviceTypeCore:
                case ParticleDeviceTypeElectron:
                case ParticleDeviceTypePhoton: // or P0 - same id
                case ParticleDeviceTypeP1:
                case ParticleDeviceTypeRedBearDuo:
                case ParticleDeviceTypeBluz:
                case ParticleDeviceTypeDigistumpOak:
                case ParticleDeviceTypeArgon:
                case ParticleDeviceTypeASeries:
                case ParticleDeviceTypeXenon:
                case ParticleDeviceTypeXSeries:
                case ParticleDeviceTypeBoron:
                case ParticleDeviceTypeBSeries:
                case ParticleDeviceTypeB5SoM:
                    _type = self.platformId;
                    break;
                default:
                    _type = ParticleDeviceTypeUnknown;
                    break;
            }
        }

        
        if ([params[@"product_id"] isKindOfClass:[NSNumber class]])
        {
            _productId = [params[@"product_id"] intValue];
        }
        
        if ((params[@"last_iccid"]) && ([params[@"last_iccid"] isKindOfClass:[NSString class]]))
        {
            _lastIccid = params[@"last_iccid"];
        }

        if ((params[@"imei"]) && ([params[@"imei"] isKindOfClass:[NSString class]]))
        {
            _imei = params[@"imei"];
        }

        if ((params[@"serial_number"]) && ([params[@"serial_number"] isKindOfClass:[NSString class]]))
        {
            _serialNumber = params[@"serial_number"];
        }

        if ((params[@"mobile_secret"]) && ([params[@"mobile_secret"] isKindOfClass:[NSString class]]))
        {
            _mobileSecret = params[@"mobile_secret"];
        }

        if ((params[@"status"]) && ([params[@"status"] isKindOfClass:[NSString class]]))
        {
            _status = params[@"status"];
        }

        
        if ([params[@"last_ip_address"] isKindOfClass:[NSString class]])
        {
            _lastIPAdress = params[@"last_ip_address"];
        }
        
        if ([params[@"last_app"] isKindOfClass:[NSString class]])
        {
            _lastApp = params[@"last_app"];
        }

        if ((params[@"notes"]) && ([params[@"notes"] isKindOfClass:[NSString class]]))
        {
            _notes = params[@"notes"];
        }

        if (params[@"network"] != nil) {
            if ((params[@"network"][@"id"]) && ([params[@"network"][@"id"] isKindOfClass:[NSString class]])) {
                _networkId = params[@"network"][@"id"];

                if ((params[@"network"][@"role"]) && (params[@"network"][@"role"][@"gateway"])) {
                    _networkRole = ([params[@"network"][@"role"][@"gateway"] boolValue]) ? ParticleDeviceNetworkRoleGateway : ParticleDeviceNetworkRoleNode;
                }

                if ((params[@"network"][@"role"]) && (params[@"network"][@"role"][@"state"]) && ([params[@"network"][@"role"][@"state"] isKindOfClass:[NSString class]])) {
                    NSString *state = [params[@"network"][@"role"][@"state"] lowercaseString];

                    if ([state isEqualToString:@"confirmed"]) {
                        _networkRoleState = ParticleDeviceNetworkRoleStatePendingConfirmed;
                    } else if ([state isEqualToString:@"pending"]) {
                        _networkRoleState = ParticleDeviceNetworkRoleStatePending;
                    } else {
                        _networkRoleState = ParticleDeviceNetworkRoleStatePendingOwnerApproval;
                    }
                }
            }
        }


        if ((params[@"system_firmware_version"]) && ([params[@"system_firmware_version"] isKindOfClass:[NSString class]]))
        {
            _systemFirmwareVersion = params[@"system_firmware_version"];
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

        if (params[@"device_needs_update"])
        {
            _requiresUpdate = YES;
        }



        [ParticleLogger logComplete:@"ParticleDevice" format:@"self = %@", self];
        return self;
    }
    
    return nil;
}

-(NSURLSessionDataTask *)ping:(nullable void(^)(BOOL result, NSError* _Nullable error))completion {
    NSURL *url = [self.baseURL URLByAppendingPathComponent:[NSString stringWithFormat:@"v1/devices/%@/ping", self.id]];

    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"PUT %@", url.absoluteString];

    [self setAuthHeaderWithAccessToken];

    NSURLSessionDataTask *task = [ParticleDevice.manager PUT:[url description] parameters:nil headers:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject) {
        [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", url.absoluteString, (int)((NSHTTPURLResponse *)task.response).statusCode];
        [ParticleLogger logDebug:NSStringFromClass([self class]) format:@"%@", responseObject];

        if (completion)
        {
            completion([responseObject[@"online"] boolValue], nil);
        }
    } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error) {
        NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];

        if (completion)
        {
            completion(nil, particleError);
        }

        [ParticleLogger logError:NSStringFromClass([self class]) format:@"! ping Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
    }];

    return task;
}

-(NSURLSessionDataTask *)refresh:(nullable ParticleCompletionBlock)completion;
{
    return [[ParticleCloud sharedInstance] getDevice:self.id completion:^(ParticleDevice * _Nullable updatedDevice, NSError * _Nullable error) {
        if (!error)
        {
            if (updatedDevice)
            {
                // if we got an updated device from the cloud - overwrite ALL self's properies with the new device properties (except for delegate which should be copied over)
                NSMutableSet *propNames = [NSMutableSet set];
                unsigned int outCount, i;
                objc_property_t *properties = class_copyPropertyList([updatedDevice class], &outCount);
                for (i = 0; i < outCount; i++) {
                    objc_property_t property = properties[i];
                    NSString *propertyName = [[NSString alloc] initWithCString:property_getName(property) encoding:NSStringEncodingConversionAllowLossy];
                    [propNames addObject:propertyName];
                }
                free(properties);
                
                if (self.delegate) {
                    updatedDevice.delegate = self.delegate;
                }
                
                for (NSString *property in propNames)
                {
                    id value = [updatedDevice valueForKey:property];
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
}

-(NSURLSessionDataTask *)getVariable:(NSString *)variableName completion:(nullable void(^)(id _Nullable result, NSError* _Nullable error))completion
{
    // TODO: check variable name exists in list
    // TODO: check response of calling a non existant function
    
    NSURL *url = [self.baseURL URLByAppendingPathComponent:[NSString stringWithFormat:@"v1/devices/%@/%@", self.id, variableName]];
    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"GET %@", url.absoluteString];

    [self setAuthHeaderWithAccessToken];
    
    NSURLSessionDataTask *task = [ParticleDevice.manager GET:[url description] parameters:nil headers:nil progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject)
    {
        [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", url.absoluteString, (int)((NSHTTPURLResponse *)task.response).statusCode];
        [ParticleLogger logDebug:NSStringFromClass([self class]) format:@"%@", responseObject];

        if (completion)
        {
            NSDictionary *responseDict = responseObject;
            if (![responseDict[@"coreInfo"][@"connected"] boolValue]) // check response
            {
                NSError *particleError = [ParticleErrorHelper getParticleError:nil task:task customMessage:@"Device is not connected"];

                completion(nil,particleError);
            }
            else
            {
                // check
                completion(responseDict[@"result"], nil);
            }
        }
    } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error)
    {
        NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];

        if (completion)
        {
            completion(nil, particleError);
        }

        [ParticleLogger logError:NSStringFromClass([self class]) format:@"! getVariable Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
    }];
    
    return task;
}

-(NSURLSessionDataTask *)callFunction:(NSString *)functionName
                        withArguments:(nullable NSArray *)args
                           completion:(nullable void (^)(NSNumber * _Nullable result, NSError * _Nullable error))completion
{
    // TODO: check function name exists in list
    // TODO: check response of calling a non existant function
    NSURL *url = [self.baseURL URLByAppendingPathComponent:[NSString stringWithFormat:@"v1/devices/%@/%@", self.id, functionName]];

    NSMutableDictionary *params = [NSMutableDictionary new]; //[self defaultParams];

    if (args) {
        NSMutableArray *argsStr = [[NSMutableArray alloc] initWithCapacity:args.count];
        for (id arg in args)
        {
            [argsStr addObject:[arg description]];
        }
        NSString *argsValue = [argsStr componentsJoinedByString:@","];
        if (argsValue.length > MAX_PARTICLE_FUNCTION_ARG_LENGTH)
        {
            NSError *particleError = [ParticleErrorHelper getParticleError:nil task:nil customMessage:[NSString stringWithFormat:@"Maximum argument length cannot exceed %d",MAX_PARTICLE_FUNCTION_ARG_LENGTH]];
            if (completion)
                completion(nil, particleError);
            return nil;
        }
            
        params[@"args"] = argsValue;
    }
    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"POST %@, params = %@", url.absoluteString, params];

    [self setAuthHeaderWithAccessToken];
    
    NSURLSessionDataTask *task = [ParticleDevice.manager POST:[url description] parameters:params headers:nil progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject)
    {
        [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", url.absoluteString, (int)((NSHTTPURLResponse *)task.response).statusCode];
        [ParticleLogger logDebug:NSStringFromClass([self class]) format:@"%@", responseObject];

        if (completion)
        {
            NSDictionary *responseDict = responseObject;
            if ([responseDict[@"connected"] boolValue]==NO)
            {
                NSError *particleError = [ParticleErrorHelper getParticleError:nil task:task customMessage:@"Device is not connected"];
                completion(nil, particleError);
            }
            else
            {
                // check
                NSNumber *result = responseDict[@"return_value"];
                completion(result,nil);
            }
        }
    } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error)
    {
        NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];

        if (completion)
        {
            completion(nil, particleError);
        }

        [ParticleLogger logError:NSStringFromClass([self class]) format:@"! callFunction Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
    }];
    
    return task;
}


-(NSURLSessionDataTask *)signal:(BOOL)enable completion:(nullable ParticleCompletionBlock)completion
{
    NSURL *url = [self.baseURL URLByAppendingPathComponent:[NSString stringWithFormat:@"v1/devices/%@", self.id]];


    NSMutableDictionary *params = [NSMutableDictionary new];
    params[@"signal"] = enable ? @"1" : @"0";
    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"PUT %@, params = %@", url.absoluteString, params];

    [self setAuthHeaderWithAccessToken];

    NSURLSessionDataTask *task = [ParticleDevice.manager PUT:[url description] parameters:params headers:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject) {
        [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", url.absoluteString, (int)((NSHTTPURLResponse *)task.response).statusCode];
        [ParticleLogger logDebug:NSStringFromClass([self class]) format:@"%@", responseObject];

        if (completion)
        {
            completion(nil);
        }
    } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error) {
        NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];

        if (completion)
        {
            completion(particleError);
        }

        [ParticleLogger logError:NSStringFromClass([self class]) format:@"! signal Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
    }];
    
    return task;
}


-(NSURLSessionDataTask *)unclaim:(nullable ParticleCompletionBlock)completion
{

    NSURL *url = [self.baseURL URLByAppendingPathComponent:[NSString stringWithFormat:@"v1/devices/%@", self.id]];
    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"DELETE %@", url.absoluteString];

    [self setAuthHeaderWithAccessToken];

    NSURLSessionDataTask *task = [ParticleDevice.manager DELETE:[url description] parameters:nil headers:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject)
    {
        [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", url.absoluteString, (int)((NSHTTPURLResponse *)task.response).statusCode];
        [ParticleLogger logDebug:NSStringFromClass([self class]) format:@"%@", responseObject];

        if (completion)
        {
            NSDictionary *responseDict = responseObject;
            if ([responseDict[@"ok"] boolValue])
                completion(nil);
            else {
                NSError *particleError = [ParticleErrorHelper getParticleError:nil task:task customMessage:@"Could not unclaim device"];

                if (completion)
                {
                    completion(particleError);
                }

                [ParticleLogger logError:NSStringFromClass([self class]) format:@"! unclaim Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
            }

        }
    } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error)
    {
        NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];

        if (completion)
        {
            completion(particleError);
        }

        [ParticleLogger logError:NSStringFromClass([self class]) format:@"! unclaim Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
    }];
    
    return task;
}

-(NSURLSessionDataTask *)rename:(NSString *)newName completion:(nullable ParticleCompletionBlock)completion
{
    NSURL *url = [self.baseURL URLByAppendingPathComponent:[NSString stringWithFormat:@"v1/devices/%@", self.id]];

    // TODO: check name validity before calling API
    NSMutableDictionary *params = [NSMutableDictionary new];
    params[@"name"] = newName;

    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"PUT %@, params = %@", url.absoluteString, params];

    [self setAuthHeaderWithAccessToken];

    NSURLSessionDataTask *task = [ParticleDevice.manager PUT:[url description] parameters:params headers:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject) {
        [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", url.absoluteString, (int)((NSHTTPURLResponse *)task.response).statusCode];
        [ParticleLogger logDebug:NSStringFromClass([self class]) format:@"%@", responseObject];

        self.name = newName;
        if (completion)
        {
            completion(nil);
        }
    } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error) {
        NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];

        if (completion)
        {
            completion(particleError);
        }

        [ParticleLogger logError:NSStringFromClass([self class]) format:@"! rename Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
    }];
    
    return task;
}

-(NSURLSessionDataTask *)setNotes:(NSString *)notes completion:(nullable ParticleCompletionBlock)completion
{
    NSURL *url = [self.baseURL URLByAppendingPathComponent:[NSString stringWithFormat:@"v1/devices/%@", self.id]];
    
    NSMutableDictionary *params = [NSMutableDictionary new];
    params[@"notes"] = notes;

    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"PUT %@, params = %@", url.absoluteString, params];

    [self setAuthHeaderWithAccessToken];

    NSURLSessionDataTask *task = [ParticleDevice.manager PUT:[url description] parameters:params headers:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject) {
        [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", url.absoluteString, (int)((NSHTTPURLResponse *)task.response).statusCode];
        [ParticleLogger logDebug:NSStringFromClass([self class]) format:@"%@", responseObject];

        self.notes = notes;
        if (completion)
        {
            completion(nil);
        }
    } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error) {
        NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];

        if (completion)
        {
            completion(particleError);
        }

        [ParticleLogger logError:NSStringFromClass([self class]) format:@"! setNotes Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
    }];

    return task;
}



#pragma mark Internal use methods
- (NSMutableDictionary *)defaultParams
{
    // TODO: change access token to be passed in header not in body
    if ([ParticleCloud sharedInstance].accessToken)
    {
        return [@{@"access_token" : [ParticleCloud sharedInstance].accessToken} mutableCopy];
    }
    else return nil;
}

-(void)setAuthHeaderWithAccessToken
{
    if ([ParticleCloud sharedInstance].accessToken) {
        NSString *authorization = [NSString stringWithFormat:@"Bearer %@",[ParticleCloud sharedInstance].accessToken];
        [ParticleDevice.manager.requestSerializer setValue:authorization forHTTPHeaderField:@"Authorization"];
    }
}


-(NSString *)description
{
    switch (self.type) {

    }

    NSString *desc = [NSString stringWithFormat:@"<ParticleDevice 0x%lx, type: %@, id: %@, name: %@, connected: %@, flashing: %@, variables: %@, functions: %@, version: %@, requires update: %@, last app: %@, last heard: %@, notes: %@, networkId: %@, networkRole: %d, networkRoleState: %d>",
                      (unsigned long)self,
                      self.typeString,
                      self.id,
                      self.name,
                      (self.connected) ? @"true" : @"false",
                      (self.isFlashing) ? @"true" : @"false",
                      self.variables,
                      self.functions,
                      self.systemFirmwareVersion,
                      (self.requiresUpdate) ? @"true" : @"false",
                      self.lastApp,
                      self.lastHeard,
                      self.notes,
                      self.networkId,
                      self.networkRole,
                      self.networkRoleState];

    return desc;
    
}


-(NSURLSessionDataTask *)flashKnownApp:(NSString *)knownAppName completion:(nullable ParticleCompletionBlock)completion
{
    NSURL *url = [self.baseURL URLByAppendingPathComponent:[NSString stringWithFormat:@"v1/devices/%@", self.id]];

    NSMutableDictionary *params = [NSMutableDictionary new];
    params[@"app"] = knownAppName;

    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"PUT %@, params = %@", url.absoluteString, params];

    [self setAuthHeaderWithAccessToken];
    
    NSURLSessionDataTask *task = [ParticleDevice.manager PUT:[url description] parameters:params headers:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject)
    {
        [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", url.absoluteString, (int)((NSHTTPURLResponse *)task.response).statusCode];
        [ParticleLogger logDebug:NSStringFromClass([self class]) format:@"%@", responseObject];

        NSDictionary *responseDict = responseObject;
        if (responseDict[@"errors"])
        {
            if (completion) {
                NSError *particleError = [ParticleErrorHelper getParticleError:nil task:task customMessage:responseDict[@"errors"][@"error"]];

                completion(particleError);
            }
        }
        else
        {
            if (completion) {
                completion(nil);
            }
        }
        
    } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error)
    {
        NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];

        if (completion)
        {
            completion(particleError);
        }

        [ParticleLogger logError:NSStringFromClass([self class]) format:@"! flashKnownApp Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
    }];
    
    return task;
}


-(nullable NSURLSessionDataTask *)flashFiles:(NSDictionary *)filesDict completion:(nullable ParticleCompletionBlock)completion // binary
{
    NSURL *url = [self.baseURL URLByAppendingPathComponent:[NSString stringWithFormat:@"v1/devices/%@", self.id]];
    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"PUT %@", url.absoluteString];

    [self setAuthHeaderWithAccessToken];
    
    NSError *reqError;
    NSMutableURLRequest *request = [ParticleDevice.manager.requestSerializer multipartFormRequestWithMethod:@"PUT" URLString:url.description parameters:@{@"file_type" : @"binary"} constructingBodyWithBlock:^(id<AFMultipartFormData> formData) {
        // check this:
        for (NSString *key in filesDict.allKeys)
        {
            [formData appendPartWithFileData:filesDict[key] name:@"file" fileName:key mimeType:@"application/octet-stream"];
        }
    } error:&reqError];
    
    if (!reqError)
    {
        __block NSURLSessionDataTask *task = [ParticleDevice.manager dataTaskWithRequest:request
                uploadProgress: nil
                downloadProgress: nil
                completionHandler:^(NSURLResponse * _Nonnull response, id  _Nullable responseObject, NSError * _Nullable error)
        {
            [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", url.absoluteString, (int)((NSHTTPURLResponse *)response).statusCode];
            [ParticleLogger logDebug:NSStringFromClass([self class]) format:@"%@", responseObject];

            if (error == nil)
            {
                NSDictionary *responseDict = responseObject;
                if (responseDict[@"error"])
                {
                    if (completion)
                    {
                        NSError *particleError = [ParticleErrorHelper getParticleError:nil task:task customMessage:responseDict[@"error"]];

                        completion(particleError);
                    }
                }
                else if (completion)
                {
                    completion(nil);
                }
            }
            else
            {
                NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];

                if (completion)
                {
                    completion(particleError);
                }

                [ParticleLogger logError:NSStringFromClass([self class]) format:@"! flashFiles Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
            }
        }];
        
        [task resume];
        return task;
    }
    else
    {
        if (completion)
        {
            completion(reqError);
        }

        return nil;
    }
}




-(nullable id)subscribeToEventsWithPrefix:(nullable NSString *)eventNamePrefix handler:(nullable ParticleEventHandler)eventHandler
{
    return [[ParticleCloud sharedInstance] subscribeToDeviceEventsWithPrefix:eventNamePrefix deviceID:self.name handler:eventHandler]; // DEBUG TODO self.id
}

-(void)unsubscribeFromEventWithID:(id)eventListenerID
{
    [[ParticleCloud sharedInstance] unsubscribeFromEventWithID:eventListenerID];
}

-(NSURLSessionDataTask *)getCurrentDataUsage:(nullable void(^)(float dataUsed, NSError* _Nullable error))completion
{
    if (self.type != ParticleDeviceTypeElectron) {
        if (completion)
        {
            NSError *particleError = [ParticleErrorHelper getParticleError:nil task:nil customMessage:@"Command supported only for Electron device"];
            completion(-1, particleError);
        }
        return nil;
    }
    
    //curl https://api.particle.io/v1/sims/8934076500002586576/data_usage\?access_token\=5451a5d6c6c54f6b20e3a109ee764596dc38a520
    NSURL *url = [self.baseURL URLByAppendingPathComponent:[NSString stringWithFormat:@"v1/sims/%@/data_usage", self.lastIccid]];
    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"GET %@", url.absoluteString];

    [self setAuthHeaderWithAccessToken];
    
    NSURLSessionDataTask *task = [ParticleDevice.manager GET:[url description] parameters:nil headers:nil progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject)
                                  {
                                      [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", url.absoluteString, (int)((NSHTTPURLResponse *)task.response).statusCode];
                                      [ParticleLogger logDebug:NSStringFromClass([self class]) format:@"%@", responseObject];

                                      if (completion)
                                      {
                                          NSDictionary *responseDict = responseObject;
                                          NSDictionary *responseUsageDict = responseDict[@"usage_by_day"];
                                          float maxUsage = 0;
                                          for (NSDictionary *usageDict in responseUsageDict) {
                                              if (usageDict[@"mbs_used_cumulative"]) {
                                                  float usage = [usageDict[@"mbs_used_cumulative"] floatValue];
                                                  if (usage > maxUsage) {
                                                      maxUsage = usage;
                                                  }
                                              }
                                          }
                                          completion(maxUsage, nil);
                                      }
                                  } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error)
                                  {
                                      NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];

                                      if (completion)
                                      {
                                          completion(-1, particleError);
                                      }

                                      [ParticleLogger logError:NSStringFromClass([self class]) format:@"! getCurrentDataUsage Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
                                  }];
    
    return task;
}

-(void)__receivedSystemEvent:(ParticleEvent *)event {
    //        {"name":"spark/status","data":"online","ttl":"60","published_at":"2016-07-13T06:20:07.300Z","coreid":"25002a001147353230333635"}
    //        {"name":"spark/flash/status","data":"started ","ttl":"60","published_at":"2016-07-13T06:30:47.130Z","coreid":"25002a001147353230333635"}
    //        {"name":"spark/flash/status","data":"success ","ttl":"60","published_at":"2016-07-13T06:30:47.702Z","coreid":"25002a001147353230333635"}
    //
    //        {"name":"spark/status/safe-mode", "data":"{\"f\":[],\"v\":{},\"p\":6,\"m\":[{\"s\":16384,\"l\":\"m\",\"vc\":30,\"vv\":30,\"f\":\"b\",\"n\":\"0\",\"v\":7,\"d\":[]},{\"s\":262144,\"l\":\"m\",\"vc\":30,\"vv\":30,\"f\":\"s\",\"n\":\"1\",\"v\":15,\"d\":[]},{\"s\":262144,\"l\":\"m\",\"vc\":30,\"vv\":30,\"f\":\"s\",\"n\":\"2\",\"v\":15,\"d\":[{\"f\":\"s\",\"n\":\"1\",\"v\":15,\"_\":\"\"}]},{\"s\":131072,\"l\":\"m\",\"vc\":30,\"vv\":26,\"u\":\"48ABD2D957D0B66069F0BCB04C8591BC8CA01FD1760F1BD47915B2C0D68070B5\",\"f\":\"u\",\"n\":\"1\",\"v\":4,\"d\":[{\"f\":\"s\",\"n\":\"2\",\"v\":17,\"_\":\"\"}]},{\"s\":131072,\"l\":\"f\",\"vc\":30,\"vv\":0,\"d\":[]}]}","ttl":"60","published_at":"2016-07-13T06:39:17.214Z","coreid":"25002a001147353230333635"}
    //        {"name":"spark/device/app-hash", "data":"48ABD2D957D0B66069F0BCB04C8591BC8CA01FD1760F1BD47915B2C0D68070B5","ttl":"60","published_at":"2016-07-13T06:39:17.215Z","coreid":"25002a001147353230333635"}
    //        {"name":"spark/status/safe-mode", "data":"{\"f\":[],\"v\":{},\"p\":6,\"m\":[{\"s\":16384,\"l\":\"m\",\"vc\":30,\"vv\":30,\"f\":\"b\",\"n\":\"0\",\"v\":7,\"d\":[]},{\"s\":262144,\"l\":\"m\",\"vc\":30,\"vv\":30,\"f\":\"s\",\"n\":\"1\",\"v\":15,\"d\":[]},{\"s\":262144,\"l\":\"m\",\"vc\":30,\"vv\":30,\"f\":\"s\",\"n\":\"2\",\"v\":15,\"d\":[{\"f\":\"s\",\"n\":\"1\",\"v\":15,\"_\":\"\"}]},{\"s\":131072,\"l\":\"m\",\"vc\":30,\"vv\":26,\"u\":\"48ABD2D957D0B66069F0BCB04C8591BC8CA01FD1760F1BD47915B2C0D68070B5\",\"f\":\"u\",\"n\":\"1\",\"v\":4,\"d\":[{\"f\":\"s\",\"n\":\"2\",\"v\":17,\"_\":\"\"}]},{\"s\":131072,\"l\":\"f\",\"vc\":30,\"vv\":0,\"d\":[]}]}","ttl":"60","published_at":"2016-07-13T06:39:17.113Z","coreid":"25002a001147353230333635"}
    //        {"name":"spark/safe-mode-updater/updating","data":"1","ttl":"60","published_at":"2016-07-13T06:39:19.467Z","coreid":"particle-internal"}
    //        {"name":"spark/safe-mode-updater/updating","data":"1","ttl":"60","published_at":"2016-07-13T06:39:19.560Z","coreid":"particle-internal"}
    //        {"name":"spark/flash/status","data":"started ","ttl":"60","published_at":"2016-07-13T06:39:21.581Z","coreid":"25002a001147353230333635"}


    NSLog(@"event.event = %@ \r\n%@", event.event, event.data);


    if ([event.event isEqualToString:@"spark/status"]) {
        if ([event.data isEqualToString:@"online"]) {
            self.connected = YES;
            self.isFlashing = NO;
            if ([self.delegate respondsToSelector:@selector(particleDevice:didReceiveSystemEvent:)]) {
                [self.delegate particleDevice:self didReceiveSystemEvent:ParticleDeviceSystemEventCameOnline];
                
            }
        }
        
        if ([event.data isEqualToString:@"offline"]) {
            self.connected = NO;
            self.isFlashing = NO;
            if ([self.delegate respondsToSelector:@selector(particleDevice:didReceiveSystemEvent:)]) {
                [self.delegate particleDevice:self didReceiveSystemEvent:ParticleDeviceSystemEventWentOffline];
            }
        }
    }
    
    if ([event.event isEqualToString:@"spark/flash/status"]) {
        if ([event.data containsString:@"started"]) {
            self.connected = YES;
            self.isFlashing = YES;
            if ([self.delegate respondsToSelector:@selector(particleDevice:didReceiveSystemEvent:)]) {
                [self.delegate particleDevice:self didReceiveSystemEvent:ParticleDeviceSystemEventFlashStarted];
                
            }
        }
        
        if ([event.data containsString:@"success"]) {
            self.connected = YES;
            self.isFlashing = NO;
            if ([self.delegate respondsToSelector:@selector(particleDevice:didReceiveSystemEvent:)]) {
                [self.delegate particleDevice:self didReceiveSystemEvent:ParticleDeviceSystemEventFlashSucceeded];
            }
        }

        if ([event.data containsString:@"failed"]) {
            self.connected = YES;
            self.isFlashing = NO;
            if ([self.delegate respondsToSelector:@selector(particleDevice:didReceiveSystemEvent:)]) {
                [self.delegate particleDevice:self didReceiveSystemEvent:ParticleDeviceSystemEventFlashFailed];
            }
        }
    }
    
    
    if ([event.event isEqualToString:@"spark/device/app-hash"]) {
        self.appHash = event.data;
        self.connected = YES;
        self.isFlashing = NO;
        if ([self.delegate respondsToSelector:@selector(particleDevice:didReceiveSystemEvent:)]) {
            [self.delegate particleDevice:self didReceiveSystemEvent:ParticleDeviceSystemEventAppHashUpdated];
        }
    }
    
    
    if ([event.event isEqualToString:@"particle/status/safe-mode"]) {
        self.connected = YES;
        self.isFlashing = YES;
        if ([self.delegate respondsToSelector:@selector(particleDevice:didReceiveSystemEvent:)]) {
            [self.delegate particleDevice:self didReceiveSystemEvent:ParticleDeviceSystemEventSafeModeUpdater];
        }
    }
    
    if ([event.event isEqualToString:@"spark/safe-mode-updater/updating"]) {
        self.connected = YES;
        self.isFlashing = YES;
        if ([self.delegate respondsToSelector:@selector(particleDevice:didReceiveSystemEvent:)]) {
            [self.delegate particleDevice:self didReceiveSystemEvent:ParticleDeviceSystemEventSafeModeUpdater];
        }
    }
    
    
    
    
    
    
}


@end

NS_ASSUME_NONNULL_END
