//
//  ParticleCloud.m
//  mobile-sdk-ios
//
//  Created by Ido Kleinman on 11/7/14.
//  Copyright (c) 2014-2015 Particle. All rights reserved.
//

#import "ParticleCloud.h"
#import "ParticleSession.h"
#import "ParticleNetwork.h"
#import "EventSource.h"
#import "ParticleErrorHelper.h"
#import "ParticleLogger.h"
#import "ParticlePricingInfo.h"
#import "ParticleDeveloperAgreement.h"

#ifdef USE_FRAMEWORKS
#import <AFNetworking/AFNetworking.h>
#else
#import "AFHTTPSessionManager.h"
#endif

NS_ASSUME_NONNULL_BEGIN

#define GLOBAL_API_TIMEOUT_INTERVAL     31.0f

NSString *const kParticleAPIBaseURL = @"https://api.particle.io";

NSString *const kEventListenersDictEventSourceKey = @"eventSource";
NSString *const kEventListenersDictHandlerKey = @"eventHandler";
NSString *const kEventListenersDictIDKey = @"id";

static NSString *const kDefaultoAuthClientId = @"particle";
static NSString *const kDefaultoAuthClientSecret = @"particle";

@interface ParticleCloud () <ParticleSessionDelegate>

@property (nonatomic, strong, nonnull) NSURL* baseURL;
@property (nonatomic, strong, nullable) ParticleSession* session;

@property (nonatomic, strong, nonnull) AFHTTPSessionManager *manager;

@property (nonatomic, strong, nonnull) NSMutableDictionary *eventListenersDict;

@property (nonatomic, strong) NSMapTable *devicesMapTable;
@property (nonatomic, strong) id systemEventsListenerId;
@end


@implementation ParticleCloud

#pragma mark Class initialization and singleton instancing

+ (instancetype)sharedInstance
{
    return [ParticleCloud sharedInstance:NO];
}

+ (instancetype)sharedInstance:(BOOL)flush
{
    static ParticleCloud *sharedInstance = nil;

    @synchronized(self) {
        if (sharedInstance == nil || flush == YES)
        {
            sharedInstance = [[self alloc] init];
        }
    }
    return sharedInstance;
}

- (instancetype)init
{
    self = [super init];
    if (self) {
        if (self.customAPIBaseURL == nil) {
            self.baseURL = [NSURL URLWithString:kParticleAPIBaseURL];
        } else {
            self.baseURL = [NSURL URLWithString:self.customAPIBaseURL];
        }

        if (!self.baseURL)
        {
            return nil;
        }

        self.oAuthClientId = kDefaultoAuthClientId;
        self.oAuthClientSecret = kDefaultoAuthClientSecret;

        // try to restore session (user and access token)
        self.session = [[ParticleSession alloc] initWithSavedSession];
        if (self.session)
        {
            self.session.delegate = self;
        }
        
        // Init HTTP manager
        self.manager = [[AFHTTPSessionManager alloc] initWithBaseURL:self.baseURL];
        self.manager.responseSerializer = [AFJSONResponseSerializer serializer];
        [self.manager.requestSerializer setTimeoutInterval:GLOBAL_API_TIMEOUT_INTERVAL];
        [self.manager.requestSerializer setValue:@"ios-cloud-sdk" forHTTPHeaderField:@"X-Particle-Tool"];
        if (!self.manager)
        {
            return nil;
        }

        // init event listeners internal dictionary
        self.eventListenersDict = [NSMutableDictionary new];

        if (self.session.accessToken != nil) {
            [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"Access Token: ...%@", [self.session.accessToken substringFromIndex:[self.session.accessToken length] - 4]];
        } else {
            [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"Access Token: nil"];
        }


        if (self.session.accessToken) {
            [self subscribeToDevicesSystemEvents];
        }
    }
    return self;
}

#pragma mark Getter functions

- (nullable NSString *)customAPIBaseURL {
    NSString *url = [[NSUserDefaults standardUserDefaults] objectForKey:@"io.particle.customAPIBaseURL"];
    return url;
}

-(nullable NSString *)accessToken
{
    return [self.session accessToken];
}

-(BOOL)injectSessionAccessToken:(NSString * _Nonnull)accessToken
{
    [self logout];
    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"Login with access token"];
    self.session = [[ParticleSession alloc] initWithToken:accessToken];
    if (self.session) {
        self.session.delegate = self;
        [self subscribeToDevicesSystemEvents];
        return YES;
    } else return NO;
}

-(BOOL)injectSessionAccessToken:(NSString *)accessToken withExpiryDate:(NSDate *)expiryDate
{
    [self logout];
    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"Login with access token"];
    self.session = [[ParticleSession alloc] initWithToken:accessToken andExpiryDate:expiryDate];
    if (self.session) {
        self.session.delegate = self;
        [self subscribeToDevicesSystemEvents];
        return YES;
    } else return NO;
}

-(BOOL)injectSessionAccessToken:(NSString *)accessToken withExpiryDate:(NSDate *)expiryDate andRefreshToken:(nonnull NSString *)refreshToken
{
    [self logout];
    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"Login with access token"];
    self.session = [[ParticleSession alloc] initWithToken:accessToken withExpiryDate:expiryDate withRefreshToken:refreshToken];
    if (self.session) {
        self.session.delegate = self;
        [self subscribeToDevicesSystemEvents];
        return YES;
    } else return NO;
}

-(nullable NSString *)loggedInUsername
{
    if ((self.session.username) && (self.session.accessToken))
    {
        return self.session.username;
    }
    else
    {
        return nil;
    }
}

- (BOOL)isLoggedIn
{
    return (self.session.username != nil);
}

- (BOOL)isAuthenticated
{
    return (self.session.accessToken != nil);
}

- (NSString *)currentBaseURL {
    return self.baseURL.absoluteString;
}

#pragma mark Setter functions

- (void)setCustomAPIBaseURL:(nullable NSString *)customApiBaseUrl {
    if (customApiBaseUrl != nil) {
        [[NSUserDefaults standardUserDefaults] setObject:customApiBaseUrl forKey:@"io.particle.customAPIBaseURL"];
    } else {
        [[NSUserDefaults standardUserDefaults] removeObjectForKey:@"io.particle.customAPIBaseURL"];
    }
    [[NSUserDefaults standardUserDefaults] synchronize];
    [ParticleCloud sharedInstance:YES];
}

- (void)setoAuthClientId:(nullable NSString *)oAuthClientId {
    _oAuthClientId = oAuthClientId ?: kDefaultoAuthClientId;
}

- (void)setoAuthClientSecret:(nullable NSString *)oAuthClientSecret {
    _oAuthClientSecret = oAuthClientSecret ?: kDefaultoAuthClientSecret;
}

#pragma mark Delegate functions

- (void)ParticleSession:(ParticleSession *)session didExpireAt:(NSDate *)date
{
    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"Session expired"];
    if (self.session.refreshToken) {
        [self refreshToken:self.session.refreshToken];
    }
    else {
        [self logout];
    }
}

- (void)refreshToken:(NSString *)refreshToken
{
    // non default params
    NSDictionary *params = @{
                             @"grant_type": @"refresh_token",
                             @"refresh_token": refreshToken
                             };
    
    [self.manager.requestSerializer setAuthorizationHeaderFieldWithUsername:self.oAuthClientId password:self.oAuthClientSecret];

    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"POST %@, params = %@", @"oauth/token", params];

    // OAuth login
    [self.manager POST:@"oauth/token" parameters:params headers:nil progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject) {
        [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", @"oauth/token", (int)((NSHTTPURLResponse *)task.response).statusCode];
        [ParticleLogger logDebug:NSStringFromClass([self class]) format:@"%@", responseObject];

        NSMutableDictionary *responseDict = [responseObject mutableCopy];
        
        if (self.session.username)
            responseDict[@"username"] = self.session.username;
        
        self.session = [[ParticleSession alloc] initWithNewSession:responseDict];
        if (self.session) // login was successful
        {
            self.session.delegate = self;
        }
    } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error) {
        NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];

        [ParticleLogger logError:NSStringFromClass([self class]) format:@"! refreshToken Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
    }];
    
    [self.manager.requestSerializer clearAuthorizationHeader];

}


#pragma mark SDK public functions

-(NSURLSessionDataTask *)loginWithUser:(NSString *)user password:(NSString *)password completion:(nullable ParticleCompletionBlock)completion
{
    // non default params
    NSDictionary *params = @{
                             @"grant_type": @"password",
                             @"username": user,
                             @"password": password,
                             };
    
    [self.manager.requestSerializer setAuthorizationHeaderFieldWithUsername:self.oAuthClientId password:self.oAuthClientSecret];
    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"POST %@", @"oauth/token"];

    // OAuth login
    NSURLSessionDataTask *task = [self.manager POST:@"oauth/token" parameters:params headers:nil progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject) {
        [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", @"oauth/token", (int)((NSHTTPURLResponse *)task.response).statusCode];
        NSMutableDictionary *responseDict = [responseObject mutableCopy];

        responseDict[@"username"] = user;
        self.session = [[ParticleSession alloc] initWithNewSession:responseDict];
        if (self.session) // login was successful
        {
            self.session.delegate = self;
            [self subscribeToDevicesSystemEvents];
        }
        
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

        [ParticleLogger logError:NSStringFromClass([self class]) format:@"! loginWithUser Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
    }];
    
    [self.manager.requestSerializer clearAuthorizationHeader];
    
    return task;
}


- (NSURLSessionDataTask *)loginWithUser:(NSString *)user mfaToken:(NSString *)mfaToken OTPToken:(NSString *)otpToken completion:(nullable ParticleCompletionBlock)completion {
    // non default params
    NSDictionary *params = @{
            @"grant_type": @"urn:custom:mfa-otp",
            @"mfa_token": mfaToken,
            @"otp": otpToken,
    };

    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"POST %@, params = %@", @"oauth/token", params];

    [self.manager.requestSerializer setAuthorizationHeaderFieldWithUsername:self.oAuthClientId password:self.oAuthClientSecret];
    NSURLSessionDataTask *task = [self.manager POST:@"oauth/token" parameters:params headers:nil progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject) {
        [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", @"oauth/token", (int)((NSHTTPURLResponse *)task.response).statusCode];

        NSMutableDictionary *responseDict = [responseObject mutableCopy];

        responseDict[@"username"] = user;
        self.session = [[ParticleSession alloc] initWithNewSession:responseDict];
        if (self.session) // login was successful
        {
            self.session.delegate = self;
            [self subscribeToDevicesSystemEvents];
        }

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

        [ParticleLogger logError:NSStringFromClass([self class]) format:@"! loginWithMFAToken Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
    }];

    [self.manager.requestSerializer clearAuthorizationHeader];

    return task;
}


-(NSURLSessionDataTask *)createUser:(NSString *)username
                           password:(NSString *)password
                        accountInfo:(nullable NSDictionary *)accountInfo
                         completion:(nullable ParticleCompletionBlock)completion
{

    //TODO: accountInfo is unclear, refactor into method having separate input parameters for everything that is hinding under account info.

    NSMutableDictionary *params = [@{
                             @"username": username,
                             @"password": password,
                             } mutableCopy];
    

    if (accountInfo) {
        params[@"account_info"] = accountInfo;
    }

    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"POST %@", @"/v1/users/"];
    NSURLSessionDataTask *task = [self.manager POST:@"/v1/users/" parameters:params headers:nil progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject)
                                  {
                                      [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", @"/v1/users/", (int)((NSHTTPURLResponse *)task.response).statusCode];
                                      NSDictionary *responseDict = responseObject;
                                      if (completion) {
                                          if ([responseDict[@"ok"] boolValue])
                                          {
                                              completion(nil);
                                          }
                                          else
                                          {
                                              NSString *errorString;
                                              if (responseDict[@"errors"][0])
                                                  errorString = [NSString stringWithFormat:@"Could not sign up: %@",responseDict[@"errors"][0]];
                                              else
                                                  errorString = @"Error signing up";

                                              NSError *particleError = [ParticleErrorHelper getParticleError:nil task:task customMessage:errorString];

                                              completion(particleError);

                                              [ParticleLogger logError:NSStringFromClass([self class]) format:@"! signupWithUser Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
                                          }
                                      }
                                  } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error)
                                  {
                                      NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];

                                      if (completion) {
                                          completion(particleError);
                                      }

                                      [ParticleLogger logError:NSStringFromClass([self class]) format:@"! signupWithUser Failed%@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
                                  }];
    
    [self.manager.requestSerializer clearAuthorizationHeader];
    
    return task;
    
}


-(NSURLSessionDataTask *)signupWithUser:(NSString *)user password:(NSString *)password completion:(nullable ParticleCompletionBlock)completion
{
    return [self createUser:user password:password accountInfo:nil completion:completion];
    
}


-(nullable NSURLSessionDataTask *)createCustomer:(NSString *)username
                                        password:(NSString *)password
                                       productId:(NSUInteger)productId
                                     accountInfo:(nullable NSDictionary *)accountInfo
                                      completion:(nullable ParticleCompletionBlock)completion
{
    // Make sure we got an orgSlug that was neither nil nor the empty string
    if (productId == 0)
    {
        if (completion)
        {
            NSError *particleError = [ParticleErrorHelper getParticleError:nil task:nil customMessage:@"productId value must be set to a non-zero value"];

            completion(particleError);
        }
        return nil;
    }
    
    if ((!self.oAuthClientId) || (!self.oAuthClientSecret))
    {
        if (completion)
        {
            NSError *particleError = [ParticleErrorHelper getParticleError:nil task:nil customMessage:@"Client OAuth credentials must be set to create a new customer"];

            completion(particleError);
        }
        return nil;
    }
    
    [self.manager.requestSerializer setAuthorizationHeaderFieldWithUsername:self.oAuthClientId password:self.oAuthClientSecret];
    
    NSMutableDictionary *params = [@{
                                     @"email": username,
                                     @"password": password,
                                     @"grant_type" : @"client_credentials",
                                     } mutableCopy];
    
    
    NSString *url = [NSString stringWithFormat:@"/v1/products/%tu/customers", productId];

    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"POST %@", url];

    NSURLSessionDataTask *task = [self.manager POST:url parameters:params headers:nil progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject)
                                  {
                                      [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", url, (int)((NSHTTPURLResponse *)task.response).statusCode];

                                      NSHTTPURLResponse *serverResponse = (NSHTTPURLResponse *)task.response;
                                      NSMutableDictionary *responseDict = [responseObject mutableCopy];
                                      //        [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"Got status code %d, and response: %@",(int)serverResponse.statusCode,responseDict];
                                      
                                      responseDict[@"username"] = username;
                                      
                                      self.session = [[ParticleSession alloc] initWithNewSession:responseDict];
                                      
                                      if (self.session) // customer login was successful
                                      {
                                          self.session.delegate = self;
                                      }
                                      
                                      if (completion)
                                      {
                                          if (serverResponse.statusCode == 201)
                                          {
                                              completion(nil);
                                          }
                                          else
                                          {
                                              NSError *particleError = [ParticleErrorHelper getParticleError:nil task:task customMessage:[responseDict[@"error"] stringValue]];

                                              completion(particleError);

                                              [ParticleLogger logError:NSStringFromClass([self class]) format:@"! createCustomer Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
                                          }
                                      }
                                  } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error)
                                  {
                                      NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];

                                      if (completion) {
                                          completion(particleError);
                                      }

                                      [ParticleLogger logError:NSStringFromClass([self class]) format:@"! createCustomer Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
                                  }];

    [self.manager.requestSerializer clearAuthorizationHeader];
    
    return task;

}


-(nullable NSURLSessionDataTask *)signupWithCustomer:(NSString *)email password:(NSString *)password orgSlug:(NSString *)orgSlug completion:(nullable ParticleCompletionBlock)completion
{
    return [self createCustomer:email password:password productId:[orgSlug integerValue] accountInfo:nil completion:completion];
}

-(void)logout
{
    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"Logout"];

    [self.session removeSession];
    [self unsubscribeToDevicesSystemEvents];
}

-(NSURLSessionDataTask *)claimDevice:(NSString *)deviceID completion:(nullable ParticleCompletionBlock)completion
{
    if (self.session.accessToken) {
        NSString *authorization = [NSString stringWithFormat:@"Bearer %@",self.session.accessToken];
        [self.manager.requestSerializer setValue:authorization forHTTPHeaderField:@"Authorization"];
    }

    NSMutableDictionary *params = [NSMutableDictionary new]; //[self defaultParams];
    params[@"id"] = deviceID;

    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"POST %@, params = %@", @"/v1/devices", params];
    
    NSURLSessionDataTask *task = [self.manager POST:@"/v1/devices" parameters:params headers:nil progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject)
    {
        [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", @"/v1/devices", (int)((NSHTTPURLResponse *)task.response).statusCode];
        [ParticleLogger logDebug:NSStringFromClass([self class]) format:@"%@", responseObject];
        if (completion)
        {
            NSMutableDictionary *responseDict = responseObject;
            
            if ([responseDict[@"ok"] boolValue])
            {
                completion(nil);
            } else
            {
                NSError *particleError = [ParticleErrorHelper getParticleError:nil task:task customMessage:@"Could not claim device"];

                if (completion) {
                    completion(particleError);
                }

                [ParticleLogger logError:NSStringFromClass([self class]) format:@"! claimDevice Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
            }
            
        }
    } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error) {
        NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];

        if (completion) {
            completion(particleError);
        }

        [ParticleLogger logError:NSStringFromClass([self class]) format:@"! claimDevice Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
    }];
    
    return task;
}

-(NSURLSessionDataTask *)getDevice:(NSString *)deviceID
                        completion:(nullable void (^)(ParticleDevice * _Nullable device, NSError * _Nullable error))completion
{
    if (self.session.accessToken) {
        NSString *authorization = [NSString stringWithFormat:@"Bearer %@",self.session.accessToken];
        [self.manager.requestSerializer setValue:authorization forHTTPHeaderField:@"Authorization"];
    }

    NSString *urlPath = [NSString stringWithFormat:@"/v1/devices/%@",deviceID];
    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"GET %@", urlPath];

    NSURLSessionDataTask *task = [self.manager GET:urlPath parameters:nil headers:nil progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject)
    {
        [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", urlPath, (int)((NSHTTPURLResponse *)task.response).statusCode];
        [ParticleLogger logComplete:NSStringFromClass([self class]) format:@"%@", responseObject];
         if (completion)
         {
             NSMutableDictionary *responseDict = responseObject;
             ParticleDevice *device = [[ParticleDevice alloc] initWithParams:responseDict];
             
             if (device) { // new 0.5.0 local storage of devices for reporting system events
                 if (!self.devicesMapTable) {
                     self.devicesMapTable = [NSMapTable mapTableWithKeyOptions:NSMapTableStrongMemory valueOptions:NSMapTableObjectPointerPersonality]; // let the user decide when to release ParticleDevice objects
                 }
                 [self.devicesMapTable setObject:device forKey:device.id];
             }
             
             if (completion)
             {
                completion(device, nil);
             }
             
         }
    } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error)
    {
        NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];

        if (completion)
        {
            completion(nil, particleError);
        }

        [ParticleLogger logError:NSStringFromClass([self class]) format:@"! getDevice Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
    }];
    
    return task;
}


-(NSURLSessionDataTask *)getDevices:(nullable void (^)(NSArray<ParticleDevice *> * _Nullable particleDevices, NSError * _Nullable error))completion
{
    if (self.session.accessToken) {
        NSString *authorization = [NSString stringWithFormat:@"Bearer %@", self.session.accessToken];
        [self.manager.requestSerializer setValue:authorization forHTTPHeaderField:@"Authorization"];
    }

    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"GET %@", @"/v1/devices"];
    NSURLSessionDataTask *task = [self.manager GET:@"/v1/devices" parameters:nil headers:nil progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject)
    {
        [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", @"/v1/devices", (int)((NSHTTPURLResponse *)task.response).statusCode];
        [ParticleLogger logComplete:NSStringFromClass([self class]) format:@"%@", responseObject];

        NSArray *responseList = responseObject;
        NSMutableArray *queryDeviceIDList = [[NSMutableArray alloc] init];
        NSMutableArray *deviceList = [[NSMutableArray alloc] init];

        // analyze
        for (NSDictionary *deviceDict in responseList)
        {
            if (deviceDict[@"id"] && ![deviceDict[@"id"] isKindOfClass:[NSNull class]])   // ignore <null> device listings that sometimes return from /v1/devices API call
            {
                ParticleDevice *device = [[ParticleDevice alloc] initWithParams:deviceDict];
                [deviceList addObject:device];

                if (device) { // new 0.5.0 local storage of devices for reporting system events
                    if (!self.devicesMapTable) {
                        self.devicesMapTable = [NSMapTable mapTableWithKeyOptions:NSMapTableStrongMemory valueOptions:NSMapTableObjectPointerPersonality]; // let the user decide when to release ParticleDevice objects
                    }
                    [self.devicesMapTable setObject:device forKey:device.id];
                }
            }
        }

        if (completion) {
            completion(deviceList, nil);
        }
    } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error)
    {
        NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];

        if (completion)
        {
            completion(nil, particleError);
        }

        [ParticleLogger logError:NSStringFromClass([self class]) format:@"! getDevices Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
    }];
    
    return task;
}


-(NSURLSessionDataTask *)getDeveloperAgreement:(nullable void(^)(ParticleDeveloperAgreement * _Nullable developerAgreement, NSError * _Nullable error))completion
{
    if (self.session.accessToken) {
        NSString *authorization = [NSString stringWithFormat:@"Bearer %@", self.session.accessToken];
        [self.manager.requestSerializer setValue:authorization forHTTPHeaderField:@"Authorization"];
    }

    NSMutableString *endpoint = [NSString stringWithFormat:@"/v1/user/service_agreements"];

    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"GET %@", endpoint];

    NSURLSessionDataTask *task = [self.manager GET:endpoint parameters:nil headers:nil progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject)
    {
        [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", endpoint, (int)((NSHTTPURLResponse *)task.response).statusCode];
        [ParticleLogger logDebug:NSStringFromClass([self class]) format:@"%@", responseObject];

        if (completion)
        {
            NSDictionary *response = responseObject;
            if (response[@"data"] != nil && ![response[@"data"] isKindOfClass:[NSNull class]]) {
                for (NSDictionary *agreementData in response[@"data"]) {
                    ParticleDeveloperAgreement *agreement = [[ParticleDeveloperAgreement alloc] initWithParams:agreementData];

                    if (agreement) {
                        completion(agreement, nil);
                        return;
                    }
                }
                completion(nil, nil);
            } else {
                NSError *particleError = [ParticleErrorHelper getParticleError:nil task:task customMessage:@"Mobile secret cannot be retrieved."];

                completion(nil, particleError);

                [ParticleLogger logError:NSStringFromClass([self class]) format:@"! getDeveloperAgreement Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
            }

        }

    } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error)
    {
        NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];

        if (completion)
        {
            completion(nil, particleError);
        }

        [ParticleLogger logError:NSStringFromClass([self class]) format:@"! getDeveloperAgreement Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
    }];

    return task;
}



-(NSURLSessionDataTask *)generateClaimCode:(nullable void(^)(NSString * _Nullable claimCode, NSArray * _Nullable userClaimedDeviceIDs, NSError * _Nullable error))completion
{
    if (self.session.accessToken) {
        NSString *authorization = [NSString stringWithFormat:@"Bearer %@",self.session.accessToken];
        [self.manager.requestSerializer setValue:authorization forHTTPHeaderField:@"Authorization"];
    }

    NSString *urlPath = [NSString stringWithFormat:@"/v1/device_claims"];

    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"POST %@", urlPath];

    NSURLSessionDataTask *task = [self.manager POST:urlPath parameters:nil headers:nil progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject)
    {
        [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", urlPath, (int)((NSHTTPURLResponse *)task.response).statusCode];
        [ParticleLogger logDebug:NSStringFromClass([self class]) format:@"%@", responseObject];

        if (completion)
        {
            NSDictionary *responseDict = responseObject;
            if (responseDict[@"claim_code"])
            {
                NSArray *claimedDeviceIDs = responseDict[@"device_ids"];
                if ((claimedDeviceIDs) && (claimedDeviceIDs.count > 0))
                {
                    completion(responseDict[@"claim_code"], responseDict[@"device_ids"], nil);
                }
                else
                {
                    completion(responseDict[@"claim_code"], nil, nil);
                }
            }
            else
            {
                NSError *particleError = [ParticleErrorHelper getParticleError:nil task:task customMessage:@"Could not generate a claim code"];

                completion(nil, nil, particleError);

                [ParticleLogger logError:NSStringFromClass([self class]) format:@"! generateClaimCode Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
            }
        }
        
    } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error)
    {
        NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];

        if (completion)
        {
            completion(nil, nil, particleError);
        }

        [ParticleLogger logError:NSStringFromClass([self class]) format:@"! generateClaimCode Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
    }];
    
    return task;
}



-(NSURLSessionDataTask *)generateClaimCodeForOrganization:(NSString *)orgSlug
                                               andProduct:(NSString *)productSlug
                                       withActivationCode:(nullable NSString *)activationCode
                                               completion:(nullable void(^)(NSString * _Nullable claimCode, NSArray * _Nullable userClaimedDeviceIDs, NSError * _Nullable error))completion
{
    return [self generateClaimCodeForProduct:[productSlug integerValue] completion:completion];
}


-(NSURLSessionDataTask *)generateClaimCodeForProduct:(NSUInteger)productId
                                          completion:(nullable void(^)(NSString *_Nullable claimCode, NSArray * _Nullable userClaimedDeviceIDs, NSError * _Nullable error))completion
{
    if (self.session.accessToken) {
        NSString *authorization = [NSString stringWithFormat:@"Bearer %@",self.session.accessToken];
        [self.manager.requestSerializer setValue:authorization forHTTPHeaderField:@"Authorization"];
    }
    
    NSString *urlPath = [NSString stringWithFormat:@"/v1/products/%tu/device_claims", productId];

    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"POST %@", @"/v1/products/%tu/device_claims"];

    NSURLSessionDataTask *task = [self.manager POST:urlPath parameters:nil headers:nil progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject)
                                  {
                                      [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", @"/v1/products/%tu/device_claims", (int)((NSHTTPURLResponse *)task.response).statusCode];
                                      [ParticleLogger logDebug:NSStringFromClass([self class]) format:@"%@", responseObject];

                                      if (completion)
                                      {
                                          NSDictionary *responseDict = responseObject;
                                          if (responseDict[@"claim_code"])
                                          {
                                              NSArray *claimedDeviceIDs = responseDict[@"device_ids"];
                                              if ((claimedDeviceIDs) && (claimedDeviceIDs.count > 0))
                                              {
                                                  completion(responseDict[@"claim_code"], responseDict[@"device_ids"], nil);
                                              }
                                              else
                                              {
                                                  completion(responseDict[@"claim_code"], nil, nil);
                                              }
                                          }
                                          else
                                          {
                                              NSError *particleError = [ParticleErrorHelper getParticleError:nil task:task customMessage:@"Could not generate a claim code"];

                                              completion(nil, nil, particleError);

                                              [ParticleLogger logError:NSStringFromClass([self class]) format:@"! generateClaimCodeForOrganization Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
                                          }
                                      }
                                      
                                  } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error)
                                  {
                                      NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];

                                      if (completion)
                                      {
                                          completion(nil, nil, particleError);
                                      }

                                      [ParticleLogger logError:NSStringFromClass([self class]) format:@"! generateClaimCodeForOrganization Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
                                  }];
    
    return task;
}


-(NSURLSessionDataTask *)requestPasswordResetForUser:(NSString *)email
                                          completion:(nullable ParticleCompletionBlock)completion
{
    NSDictionary *params = @{@"username": email};
    NSString *urlPath = [NSString stringWithFormat:@"/v1/user/password-reset"];

    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%POST @, params = %@", urlPath, params];


    NSURLSessionDataTask *task = [self.manager POST:urlPath parameters:params headers:nil progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject)
    {
        [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", urlPath, (int)((NSHTTPURLResponse *)task.response).statusCode];
        [ParticleLogger logDebug:NSStringFromClass([self class]) format:@"%@", responseObject];

        if (completion) // TODO: check responses
        {
            completion(nil);
        }
    } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error) {
        NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];

        if (completion)
        {
            completion(particleError);
        }

        [ParticleLogger logError:NSStringFromClass([self class]) format:@"! requestPasswordResetForUser Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
    }];
    
    return task;
}


#pragma mark Mesh BLE OTA Update
-(NSURLSessionDataTask *)getNextBinaryURL:(ParticleDeviceType)deviceType currentSystemFirmwareVersion:(NSString *)currentSystemFirmwareVersion currentNcpFirmwareVersion:(NSString * _Nullable)currentNcpFirmwareVersion currentNcpFirmwareModuleVersion:(NSNumber * _Nullable)currentNcpFirmwareModuleVersion  completion:(nullable void(^)(NSString * _Nullable binaryURL, NSError* _Nullable error))completion
{
    if (self.session.accessToken) {
        NSString *authorization = [NSString stringWithFormat:@"Bearer %@",self.session.accessToken];
        [self.manager.requestSerializer setValue:authorization forHTTPHeaderField:@"Authorization"];
    }

    NSURL *url = [self.baseURL URLByAppendingPathComponent:@"v1/system_firmware/upgrade"];
    NSMutableDictionary *params = [[NSMutableDictionary alloc] initWithCapacity:2];
    params[@"platform_id"] = @(deviceType);
    params[@"current_system_firmware_version"] = currentSystemFirmwareVersion;

    if (currentNcpFirmwareVersion != nil) {
        params[@"current_ncp_firmware_version"] = currentNcpFirmwareVersion;
    }

    if (currentNcpFirmwareModuleVersion != nil) {
        params[@"current_ncp_firmware_module_version"] = currentNcpFirmwareModuleVersion;
    }

    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"GET %@, params = %@", url.absoluteString, params];


    NSURLSessionDataTask *task = [self.manager GET:[url description] parameters:params headers:nil progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject)
    {
        [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", url.absoluteString, (int)((NSHTTPURLResponse *)task.response).statusCode];
        [ParticleLogger logDebug:NSStringFromClass([self class]) format:@"%@", responseObject];

        if (completion)
        {
            NSHTTPURLResponse *serverResponse = (NSHTTPURLResponse *)task.response;
            if (serverResponse.statusCode == 200) {
                NSDictionary *responseDict = responseObject;
                completion(responseDict[@"binary_url"], nil);
            } else { //if (serverResponse.statusCode == 204) {
                completion(nil, nil);
            }

        }
    } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error) {
        NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];

        if (completion)
        {
            completion(nil, particleError);
        }

        [ParticleLogger logError:NSStringFromClass([self class]) format:@"! getNextBinaryURL Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
    }];

    return task;
}

- (NSURLSessionDataTask *)getNextBinary:(NSString *)url completion:(nullable void(^)(NSString * _Nullable binaryFilePath, NSError* _Nullable error))completion {
    NSURL *URL = [NSURL URLWithString:url];
    NSURLRequest *request = [NSURLRequest requestWithURL:URL];


    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"DOWNLOAD %@", URL.absoluteString];

    NSURLSessionDownloadTask *task = [self.manager downloadTaskWithRequest:request progress:nil
            destination:^NSURL *(NSURL *targetPath, NSURLResponse *response) {
        NSURL *cachesDirectoryURL = [[NSFileManager defaultManager] URLForDirectory:NSCachesDirectory inDomain:NSUserDomainMask appropriateForURL:nil create:NO error:nil];
        return [cachesDirectoryURL URLByAppendingPathComponent:[response suggestedFilename]];
    } completionHandler:^(NSURLResponse *response, NSURL *filePath, NSError *error) {
                [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", URL.absoluteString, (int)((NSHTTPURLResponse *)task.response).statusCode];

        if (completion)
        {
            [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"getNextBinary error = %@", error];
            [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"getNextBinary filePath = %@", filePath.absoluteString];
            if (error == nil) {
                completion(filePath.absoluteString, nil);
            } else {
                NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];

                if (completion)
                {
                    completion(nil, particleError);
                }

                [ParticleLogger logError:NSStringFromClass([self class]) format:@"! getNextBinary Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
            }
        }
    }];
    [task resume];

    return task;
}

#pragma mark Internal use methods

-(NSURLSessionDataTask *)listTokens:(NSString *)user password:(NSString *)password
{
    [self.manager.requestSerializer setAuthorizationHeaderFieldWithUsername:user password:password];

    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"GET %@", @"/v1/access_tokens"];

    NSURLSessionDataTask *task = [self.manager GET:@"/v1/access_tokens" parameters:nil headers:nil progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject)
    {
        [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", @"/v1/access_tokens", (int)((NSHTTPURLResponse *)task.response).statusCode];
        [ParticleLogger logDebug:NSStringFromClass([self class]) format:@"%@", responseObject];

    } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error)
    {
        [ParticleLogger logError:NSStringFromClass([self class]) format:@"listTokens %@",[error localizedDescription]];
    }];
    
    [self.manager.requestSerializer clearAuthorizationHeader];
    
    return task;
}

#pragma mark Events subsystem implementation

-(nullable id)subscribeToEventWithURL:(NSURL *)url handler:(nullable ParticleEventHandler)eventHandler
{
    if (!self.accessToken)
    {
        NSError *particleError = [ParticleErrorHelper getParticleError:nil task:nil customMessage:@"No active access token"];

        eventHandler(nil, particleError);
        return nil;
    }

    // TODO: add eventHandler + source to an internal dictionary so it will be removeable later by calling removeEventListener on saved Source
    EventSource *source = [EventSource eventSourceWithURL:url timeoutInterval:300.0f queue:dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0)];
    
    //    if (eventName == nil)
    //        eventName = @"no_name";
    
    // - event example -
    // event: Temp
    // data: {"data":"Temp1 is 41.900002 F, Temp2 is $f F","ttl":"60","published_at":"2015-01-13T01:23:12.269Z","coreid":"53ff6e066667574824151267"}
    
    //    [source addEventListener:@"" handler:^(Event *event) { //event name
//    [source onMessage:
    
     EventSourceEventHandler handler = ^void(Event *event) {
        if (eventHandler)
        {
            if (event.error)
                eventHandler(nil, event.error);
            else
            {
                // deserialize event payload into dictionary
                NSError *error;
                NSDictionary *jsonDict;
                NSMutableDictionary *eventDict;
                if (event.data)
                {
                    jsonDict = [NSJSONSerialization JSONObjectWithData:event.data options:0 error:&error];
                    eventDict = [jsonDict mutableCopy];
                }
                
                if ((eventDict) && (!error))
                {
                    if (event.name)
                    {
                        eventDict[@"event"] = event.name; // add event name to dict
                    }
                    ParticleEvent *particleEvent = [[ParticleEvent alloc] initWithEventDict:eventDict];
                    eventHandler(particleEvent ,nil); // callback with parsed data
                }
                else if (error)
                {
                    eventHandler(nil, error);
                }
            }
        }
        
    };
    
    [source onMessage:handler]; // bind the handler
    
    id eventListenerID = [NSUUID UUID]; // create the eventListenerID
    self.eventListenersDict[eventListenerID] = @{kEventListenersDictHandlerKey : handler,
                                                 kEventListenersDictEventSourceKey : source}; // save it in the internal dictionary for future unsubscribing
    
    return eventListenerID;
    
}


-(void)unsubscribeFromEventWithID:(id)eventListenerID
{
    NSDictionary *eventListenerDict = [self.eventListenersDict objectForKey:eventListenerID];
    if (eventListenerDict)
    {
        EventSource *source = [eventListenerDict objectForKey:kEventListenersDictEventSourceKey];
        EventSourceEventHandler handler = [eventListenerDict objectForKey:kEventListenersDictHandlerKey];
        [source removeEventListener:ParticleMessageEvent handler:handler];
        [source close];
        [self.eventListenersDict removeObjectForKey:eventListenerID];
    }
}


-(nullable id)subscribeToAllEventsWithPrefix:(nonnull NSString *)eventNamePrefix handler:(nullable ParticleEventHandler)eventHandler
{
    // GET /v1/events[/:event_name]
    NSString *endpoint;
    if ((!eventNamePrefix) || [eventNamePrefix isEqualToString:@""])
    {
        [ParticleLogger logError:NSStringFromClass([self class]) format:@"! subscribeToAllEventsWithPrefix Failed: eventNamePrefix is no longer optional and cannot be empty for this event stream."];
        return nil;
    }
    else
    {
        // URL encode name prefix
        NSCharacterSet *set = [NSCharacterSet URLHostAllowedCharacterSet];
        NSString *encodedEventPrefix = [eventNamePrefix stringByAddingPercentEncodingWithAllowedCharacters:set];
        endpoint = [NSString stringWithFormat:@"%@/v1/events/%@?access_token=%@", self.baseURL, encodedEventPrefix, self.accessToken];

        [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"Subscribe to events with URL = %@", [NSString stringWithFormat:@"%@/v1/events/%@?access_token=%@", self.baseURL, encodedEventPrefix, @"ACCESS_TOKEN"]];
    }
    
    return [self subscribeToEventWithURL:[NSURL URLWithString:endpoint] handler:eventHandler];
}


-(nullable id)subscribeToMyDevicesEventsWithPrefix:(nullable NSString *)eventNamePrefix handler:(nullable ParticleEventHandler)eventHandler
{
    // GET /v1/devices/events[/:event_name]
    NSString *endpoint;
    if ((!eventNamePrefix) || [eventNamePrefix isEqualToString:@""])
    {
        endpoint = [NSString stringWithFormat:@"%@/v1/devices/events?access_token=%@", self.baseURL, self.accessToken];
    }
    else
    {
        // URL encode name prefix
        NSCharacterSet *set = [NSCharacterSet URLHostAllowedCharacterSet];
        NSString *encodedEventPrefix = [eventNamePrefix stringByAddingPercentEncodingWithAllowedCharacters:set];
        endpoint = [NSString stringWithFormat:@"%@/v1/devices/events/%@?access_token=%@", self.baseURL, encodedEventPrefix, self.accessToken];

        [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"Subscribe to events with URL = %@", [NSString stringWithFormat:@"%@/v1/devices/events/%@?access_token=%@", self.baseURL, encodedEventPrefix, @"ACCESS_TOKEN"]];
    }
    
    return [self subscribeToEventWithURL:[NSURL URLWithString:endpoint] handler:eventHandler];
    
}

-(nullable id)subscribeToDeviceEventsWithPrefix:(nullable NSString *)eventNamePrefix deviceID:(NSString *)deviceID handler:(nullable ParticleEventHandler)eventHandler
{
    // GET /v1/devices/:device_id/events[/:event_name]
    NSString *endpoint;
    if ((!eventNamePrefix) || [eventNamePrefix isEqualToString:@""])
    {
        endpoint = [NSString stringWithFormat:@"%@/v1/devices/%@/events?access_token=%@", self.baseURL, deviceID, self.accessToken];
    }
    else
    {
        // URL encode name prefix
        NSCharacterSet *set = [NSCharacterSet URLHostAllowedCharacterSet];
        NSString *encodedEventPrefix = [eventNamePrefix stringByAddingPercentEncodingWithAllowedCharacters:set];
        endpoint = [NSString stringWithFormat:@"%@/v1/devices/%@/events/%@?access_token=%@", self.baseURL, deviceID, encodedEventPrefix, self.accessToken];

        [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"Subscribe to events with URL = %@", [NSString stringWithFormat:@"%@/v1/devices/%@/events/%@?access_token=%@", self.baseURL, deviceID, encodedEventPrefix, @"ACCESS_TOKEN"]];
    }
    
    return [self subscribeToEventWithURL:[NSURL URLWithString:endpoint] handler:eventHandler];
}



-(NSURLSessionDataTask *)publishEventWithName:(NSString *)eventName
                                         data:(NSString *)data
                                    isPrivate:(BOOL)isPrivate
                                          ttl:(NSUInteger)ttl
                                   completion:(nullable ParticleCompletionBlock)completion
{
    NSMutableDictionary *params = [NSMutableDictionary new];
    if (self.session.accessToken) {
        NSString *authorization = [NSString stringWithFormat:@"Bearer %@",self.session.accessToken];
        [self.manager.requestSerializer setValue:authorization forHTTPHeaderField:@"Authorization"];
    }
    
    params[@"name"] = eventName;
    params[@"data"] = data;
    params[@"private"] = isPrivate ? @"true" : @"false";
    params[@"ttl"] = [NSString stringWithFormat:@"%lu", (unsigned long)ttl];

    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"POST %@, params = %@", @"/v1/devices/events", params];

    NSURLSessionDataTask *task = [self.manager POST:@"/v1/devices/events" parameters:params headers:nil progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject)
    {
        [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", @"/v1/devices/events", (int)((NSHTTPURLResponse *)task.response).statusCode];
        [ParticleLogger logDebug:NSStringFromClass([self class]) format:@"%@", responseObject];

        if (completion)
        {
            // TODO: check server response for that
            NSDictionary *responseDict = responseObject;
            if (![responseDict[@"ok"] boolValue])
            {
                NSError *particleError = [ParticleErrorHelper getParticleError:nil task:task customMessage:@"Server reported error publishing event"];

                completion(particleError);

                [ParticleLogger logError:NSStringFromClass([self class]) format:@"! publishEventWithName Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
            }
            else
            {
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

        [ParticleLogger logError:NSStringFromClass([self class]) format:@"! publishEventWithName Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
    }];
    
    return task;
}



-(void)subscribeToDevicesSystemEvents {
    
    __weak ParticleCloud *weakSelf = self;
    self.systemEventsListenerId = [self subscribeToMyDevicesEventsWithPrefix:@"spark" handler:^(ParticleEvent * _Nullable event, NSError * _Nullable error) {

        if (!error) {
            ParticleDevice *device = [weakSelf.devicesMapTable objectForKey:event.deviceID];
            if (device) {
                [device __receivedSystemEvent:event];
            }
        } else {
            [ParticleLogger logError:NSStringFromClass([self class]) format:@"! ParticleCloud could not subscribeToEvents to devices system events %@",error.localizedDescription];
        }
    }];

}

-(void)unsubscribeToDevicesSystemEvents {
    if (self.systemEventsListenerId) {
        [self unsubscribeFromEventWithID:self.systemEventsListenerId];
    }
}

-(NSURLSessionDataTask *)getCard:(nullable void(^)(NSString* _Nullable token, NSString* _Nullable last4, NSUInteger expiryMonth, NSUInteger expiryYear, NSString* _Nullable brand, NSError * _Nullable error))completion;

{
    if (self.session.accessToken) {
        NSString *authorization = [NSString stringWithFormat:@"Bearer %@", self.session.accessToken];
        [self.manager.requestSerializer setValue:authorization forHTTPHeaderField:@"Authorization"];
    }

    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@", @"GET /v1/card"];

    NSURLSessionDataTask *task = [self.manager GET:@"/v1/card" parameters:nil headers:nil progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject)
                                  {
                                      [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", @"/v1/card", (int)((NSHTTPURLResponse *)task.response).statusCode];
                                      [ParticleLogger logDebug:NSStringFromClass([self class]) format:@"%@", responseObject];

                                      if (completion)
                                      {
                                          NSDictionary *responseDict = responseObject;
                                          if ([responseDict[@"card"] isKindOfClass:[NSDictionary class]]) {
                                              NSDictionary *card = responseDict[@"card"];
                                              NSString* last4 = card[@"last4"];
                                              NSString* brand = card[@"brand"];
                                              NSString* expiryMonthString = card[@"exp_month"];
                                              NSString* expiryYearString = card[@"exp_year"];
                                              NSString* token = card[@"id"];
                                              completion(token, last4, [expiryMonthString integerValue], [expiryYearString integerValue], brand, nil);
                                          } else {
                                              NSString *errorString;
                                              if (responseDict[@"error"])
                                                  errorString = [NSString stringWithFormat:@"Could not get card: %@",responseDict[@"error"]];
                                              else
                                                  errorString = @"Could not get card";

                                              NSError *particleError = [ParticleErrorHelper getParticleError:nil task:task customMessage:errorString];

                                              completion(nil, nil, 0,0, nil, particleError);

                                              [ParticleLogger logError:NSStringFromClass([self class]) format:@"! getCard Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
                                          }
                                      }

                                  } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error)
                                  {
                                      NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];

                                      if (completion)
                                      {
                                          completion(nil, nil, 0,0, nil, particleError);
                                      }

                                      [ParticleLogger logError:NSStringFromClass([self class]) format:@"! getCard Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
                                  }];

    return task;
}



-(NSURLSessionDataTask *)getNetworks:(nullable void(^)(NSArray<ParticleNetwork *> * _Nullable networks, NSError * _Nullable error))completion
{
    if (self.session.accessToken) {
        NSString *authorization = [NSString stringWithFormat:@"Bearer %@", self.session.accessToken];
        [self.manager.requestSerializer setValue:authorization forHTTPHeaderField:@"Authorization"];
    }

    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"GET %@", @"/v1/networks"];

    NSURLSessionDataTask *task = [self.manager GET:@"/v1/networks" parameters:nil headers:nil progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject)
                                  {
                                      [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", @"/v1/networks", (int)((NSHTTPURLResponse *)task.response).statusCode];
                                      [ParticleLogger logDebug:NSStringFromClass([self class]) format:@"%@", responseObject];

                                      if (completion)
                                      {
                                          NSArray *responseList = responseObject;
                                          __block NSMutableArray *networksList = [[NSMutableArray alloc] init];
                                          
                                          for (NSDictionary *networkDict in responseList)
                                          {
                                              if (networkDict[@"id"])   // ignore <null> device listings that sometimes return from /v1/devices API call
                                              {
                                                  if (![networkDict[@"id"] isKindOfClass:[NSNull class]])
                                                  {
                                                      ParticleNetwork *network = [[ParticleNetwork alloc] initWithParams:networkDict];
                                                      if (network) {
                                                          [networksList addObject:network];
                                                      }
                                                  }
                                              }
                                              
                                          }
                                          
                                          if (networksList.count == 0) {
                                              // no networks returned
                                              completion(nil, nil);
                                          } else {
                                              completion(networksList, nil);
                                          }
                                      }
                                      
                                  } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error)
                                  {
                                      NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];
                                      
                                      if (completion)
                                      {
                                          completion(nil, particleError);
                                      }

                                      [ParticleLogger logError:NSStringFromClass([self class]) format:@"! getNetworks Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
                                  }];
    
    return task;
}



-(NSURLSessionDataTask *)getNetwork:(NSString *)idOrName
                         completion:(nullable void(^)(ParticleNetwork * _Nullable network, NSError * _Nullable error))completion
{
    if (self.session.accessToken) {
        NSString *authorization = [NSString stringWithFormat:@"Bearer %@", self.session.accessToken];
        [self.manager.requestSerializer setValue:authorization forHTTPHeaderField:@"Authorization"];
    }
    
    NSString *endpoint = [NSString stringWithFormat:@"/v1/networks/%@", idOrName];

    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"GET %@", endpoint];

    NSURLSessionDataTask *task = [self.manager GET:endpoint parameters:nil headers:nil progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject)
                                  {
                                      [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", endpoint, (int)((NSHTTPURLResponse *)task.response).statusCode];
                                      [ParticleLogger logDebug:NSStringFromClass([self class]) format:@"%@", responseObject];

                                      if (completion)
                                      {
                                          ParticleNetwork *network = nil;
                                          NSDictionary *response = responseObject;
                                              if (response[@"id"])   // ignore <null> device listings that sometimes return from /v1/devices API call
                                              {
                                                  if (![response[@"id"] isKindOfClass:[NSNull class]])
                                                  {
                                                      network = [[ParticleNetwork alloc] initWithParams:response];
                                                  }
                                              }
                                              
                                          completion(network, nil);
                                      }
                                      
                                  } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error)
                                  {
                                      NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];
                                      
                                      if (completion)
                                      {
                                          completion(nil, particleError);
                                      }

                                      [ParticleLogger logError:NSStringFromClass([self class]) format:@"! getNetwork Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
                                  }];
    
    return task;
    
}

-(NSURLSessionDataTask *)getRecoveryMobileSecret:(NSString *  _Nonnull)serialNumber
                       mobileSecret:(NSString * _Nonnull)mobileSecret
                         completion:(nullable void(^)(NSString * _Nullable mobileSecret, NSError * _Nullable error))completion
{
    if (self.session.accessToken) {
        NSString *authorization = [NSString stringWithFormat:@"Bearer %@", self.session.accessToken];
        [self.manager.requestSerializer setValue:authorization forHTTPHeaderField:@"Authorization"];
    }

    NSMutableString *endpoint = [NSString stringWithFormat:@"/v1/serial_numbers/%@?mobile_secret=%@", serialNumber, mobileSecret];

    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"GET %@", endpoint];

    NSURLSessionDataTask *task = [self.manager GET:endpoint parameters:nil headers:nil progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject)
                                  {
                                      [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", endpoint, (int)((NSHTTPURLResponse *)task.response).statusCode];
                                      [ParticleLogger logDebug:NSStringFromClass([self class]) format:@"%@", responseObject];

                                      if (completion)
                                      {
                                          NSDictionary *response = responseObject;
                                          if (response[@"mobile_secret"] != nil && ![response[@"mobile_secret"] isKindOfClass:[NSNull class]]) {
                                              completion(response[@"mobile_secret"], nil);
                                          } else {
                                              NSError *particleError = [ParticleErrorHelper getParticleError:nil task:task customMessage:@"Mobile secret cannot be retrieved."];

                                              completion(nil, particleError);

                                              [ParticleLogger logError:NSStringFromClass([self class]) format:@"! getRecoveryMobileSecret Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
                                          }

                                      }

                                  } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error)
                                  {
                                      NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];

                                      if (completion)
                                      {
                                          completion(nil, particleError);
                                      }

                                      [ParticleLogger logError:NSStringFromClass([self class]) format:@"! getRecoveryMobileSecret Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
                                  }];

    return task;
}


-(NSURLSessionDataTask *)getPlatformId:(NSString *  _Nonnull)serialNumber
                                      completion:(nullable void(^)(NSNumber * _Nullable platformId, NSError * _Nullable error))completion
{
    if (self.session.accessToken) {
        NSString *authorization = [NSString stringWithFormat:@"Bearer %@", self.session.accessToken];
        [self.manager.requestSerializer setValue:authorization forHTTPHeaderField:@"Authorization"];
    }

    NSMutableString *endpoint = [NSString stringWithFormat:@"/v1/serial_numbers/%@", serialNumber];

    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"GET %@", endpoint];

    NSURLSessionDataTask *task = [self.manager GET:endpoint parameters:nil headers:nil progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject)
    {
        [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", endpoint, (int)((NSHTTPURLResponse *)task.response).statusCode];
        [ParticleLogger logDebug:NSStringFromClass([self class]) format:@"%@", responseObject];

        if (completion)
        {
            NSDictionary *response = responseObject;
            if (response[@"platform_id"] != nil && ![response[@"platform_id"] isKindOfClass:[NSNull class]]) {
                completion(@([response[@"platform_id"] intValue]), nil);
            } else {
                NSError *particleError = [ParticleErrorHelper getParticleError:nil task:task customMessage:@"Platform ID cannot be retrieved."];

                completion(nil, particleError);

                [ParticleLogger logError:NSStringFromClass([self class]) format:@"! getPlatformId Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
            }

        }

    } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error)
    {
        NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];

        if (completion)
        {
            completion(nil, particleError);
        }

        [ParticleLogger logError:NSStringFromClass([self class]) format:@"! getRecoveryMobileSecret Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
    }];

    return task;
}




-(NSURLSessionDataTask *)createNetwork:(NSString *)networkName
                       gatewayDeviceID:(NSString *)gatewayDeviceID
                    gatewayDeviceICCID:(NSString * _Nullable)gatewayDeviceICCID
                           networkType:(ParticleNetworkType)networkType
                            completion:(nullable void(^)(ParticleNetwork * _Nullable network, NSError * _Nullable error))completion
{
    if (self.session.accessToken) {
        NSString *authorization = [NSString stringWithFormat:@"Bearer %@",self.session.accessToken];
        [self.manager.requestSerializer setValue:authorization forHTTPHeaderField:@"Authorization"];
    }

    NSMutableDictionary *params = [@{
                                     @"name" : networkName,
                                     @"deviceID": gatewayDeviceID,
                                     } mutableCopy];
    
    // optional ICCID for borons
    if (gatewayDeviceICCID) {
        params[@"iccid"] = gatewayDeviceICCID;
    }
    
    switch (networkType) {
        case ParticleNetworkTypeMicroWifi:
            params[@"type"] = @"micro_wifi";
            break;

        case ParticleNetworkTypeMicroCellular:
            params[@"type"] = @"micro_cellular";
            break;

        case ParticleNetworkTypeHighAvailability:
            params[@"type"] = @"high_availability";
            break;

        case ParticleNetworkTypeLargeSite:
            params[@"type"] = @"large_site";
            break;
            
    }

    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"POST %@, params = %@", @"/v1/networks/", params];

    NSURLSessionDataTask *task = [self.manager POST:@"/v1/networks/" parameters:params headers:nil progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject)
                                  {
                                      [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", @"/v1/networks/", (int)((NSHTTPURLResponse *)task.response).statusCode];
                                      [ParticleLogger logDebug:NSStringFromClass([self class]) format:@"%@", responseObject];

                                      NSDictionary *responseDict = responseObject;
                                      if (completion) {
                                          if (([responseDict[@"ok"] boolValue]) && (![responseDict[@"network"] isKindOfClass:[NSNull class]]))
                                          {
                                              ParticleNetwork* network = [[ParticleNetwork alloc] initWithParams:responseDict[@"network"]];
                                              completion(network, nil);
                                          }
                                          else
                                          {
                                              NSString *errorString;
                                              if (responseDict[@"errors"][0])
                                                  errorString = [NSString stringWithFormat:@"Could not create network: %@",responseDict[@"errors"][0]];
                                              else
                                                  errorString = @"Error creating network";
                                              
                                              NSError *particleError = [ParticleErrorHelper getParticleError:nil task:task customMessage:errorString];
                                              
                                              completion(nil, particleError);

                                              [ParticleLogger logError:NSStringFromClass([self class]) format:@"! createNetwork Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
                                          }
                                      }
                                  } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error)
                                  {
                                      NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];
                                      
                                      if (completion) {
                                          completion(nil, particleError);
                                      }

                                      [ParticleLogger logError:NSStringFromClass([self class]) format:@"! createNetwork Failed%@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
                                  }];
    
    [self.manager.requestSerializer clearAuthorizationHeader];
    
    return task;
    
}

-(NSURLSessionDataTask *)addCard:(NSString *)stripeTokenId completion:(nullable void(^)(NSError * _Nullable))completion{
    if (self.session.accessToken) {
        NSString *authorization = [NSString stringWithFormat:@"Bearer %@", self.session.accessToken];
        [self.manager.requestSerializer setValue:authorization forHTTPHeaderField:@"Authorization"];
    }

    NSMutableDictionary *params = [[NSMutableDictionary alloc] init];
    params[@"stripe_token"] = stripeTokenId;


    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"PUT %@, params = %@", @"/v1/card/", params];

    NSURLSessionDataTask *task = [self.manager PUT:@"/v1/card/" parameters:params headers:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject)
    {
        [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", @"/v1/card/", (int)((NSHTTPURLResponse *)task.response).statusCode];
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

        [ParticleLogger logError:NSStringFromClass([self class]) format:@"! addCard Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
    }];


    return task;
}

-(NSURLSessionDataTask *)getSim:(NSString *)iccid completion:(nullable void(^)(ParticleSimInfo * _Nullable, NSError * _Nullable))completion
{
    if (self.session.accessToken) {
        NSString *authorization = [NSString stringWithFormat:@"Bearer %@", self.session.accessToken];
        [self.manager.requestSerializer setValue:authorization forHTTPHeaderField:@"Authorization"];
    }
    NSString *url = [NSString stringWithFormat:@"/v1/sims/%@/", iccid];

    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"GET %@", url];


    NSURLSessionDataTask *task = [self.manager GET:[url description] parameters:nil headers:nil progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject)
    {
        [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", url, (int)((NSHTTPURLResponse *)task.response).statusCode];
        [ParticleLogger logDebug:NSStringFromClass([self class]) format:@"%@", responseObject];

        if (completion)
        {
            NSHTTPURLResponse *serverResponse = (NSHTTPURLResponse *)task.response;
            NSDictionary *responseDict = responseObject;
            completion([[ParticleSimInfo alloc] initWithParams:responseDict], nil);
        }
    } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error) {
        NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];

        if (completion)
        {
            completion(nil, particleError);
        }

        [ParticleLogger logError:NSStringFromClass([self class]) format:@"! getSim Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
    }];


    return task;
}


-(NSURLSessionDataTask *)checkSim:(NSString *)iccid completion:(nullable void(^)(ParticleSimStatus simStatus, NSError * _Nullable))completion
{
    if (self.session.accessToken) {
        NSString *authorization = [NSString stringWithFormat:@"Bearer %@", self.session.accessToken];
        [self.manager.requestSerializer setValue:authorization forHTTPHeaderField:@"Authorization"];
    }
    NSString *url = [NSString stringWithFormat:@"/v1/sims/%@/", iccid];


    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"HEAD %@", url];


    NSURLSessionDataTask *task = [self.manager HEAD:url parameters:nil headers:nil success:^(NSURLSessionDataTask * _Nonnull task)
                                  {
                                      [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", url, (int)((NSHTTPURLResponse *)task.response).statusCode];

                                      if (completion)
                                      {
                                          int code = -1;
                                          NSHTTPURLResponse *serverResponse = (NSHTTPURLResponse *)task.response;
                                          code = (int)serverResponse.statusCode;
                                          
                                          switch (code) {
                                              case 204:
                                                  completion(ParticleSimStatusInactive, nil);
                                                  break;

                                              case 205:
                                                  completion(ParticleSimStatusActive, nil);
                                                  break;

                                              default: // 200
                                                  completion(ParticleSimStatusInactive, nil);
                                                  break;
                                          }
                                      }
                                      
                                  } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error)
                                  {
                                      NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];

                                      if (completion) {
                                          NSHTTPURLResponse *serverResponse = (NSHTTPURLResponse *)task.response;
                                          int code = (int)serverResponse.statusCode;

                                          switch (code) {
                                              case 404:
                                                  completion(ParticleSimStatusNotFound, error);
                                                  break;

                                              case 403:
                                                  completion(ParticleSimStatusNotOwnedByUser, error);
                                                  break;

                                              default: {
                                                  completion(ParticleSimStatusError, error); //particleError);
                                                  break;
                                              }
                                          }

                                          completion(nil, particleError);
                                      }

                                      [ParticleLogger logError:NSStringFromClass([self class]) format:@"! createNetwork Failed%@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
                                  }];
    
    return task;
}




-(NSURLSessionDataTask *)updateSim:(NSString *)iccid action:(ParticleUpdateSimAction)action dataLimit:(NSNumber * _Nullable)dataLimit countryCode:(NSString * _Nullable)countryCode cardToken:(NSString * _Nullable)cardToken completion:(nullable ParticleCompletionBlock)completion
{
    if (self.session.accessToken) {
        NSString *authorization = [NSString stringWithFormat:@"Bearer %@",self.session.accessToken];
        [self.manager.requestSerializer setValue:authorization forHTTPHeaderField:@"Authorization"];
    }
    
    NSString *actionString;
    NSString *mb_limit;
    NSMutableDictionary *errorDetail = [[NSMutableDictionary alloc] init];
    
    switch (action) {
        case ParticleUpdateSimActionActivate:
            actionString = @"activate";
            break;

        case ParticleUpdateSimActionDeactivate:
            actionString = @"deactivate";
            break;
            
        case ParticleUpdateSimActionReactivate:
            actionString = @"reactivate";
            if (dataLimit) {
                mb_limit = [dataLimit stringValue];
            }
            break;
            
        case ParticleUpdateSimActionSetDataLimit:
            if (dataLimit) {
                mb_limit = [dataLimit stringValue];
            } else {
                [errorDetail setValue:@"Must pass valid integer dataLimit when action is SetDataLimit" forKey:NSLocalizedDescriptionKey];
                NSError *error = [NSError errorWithDomain:@"ParticleAPIError" code:0 userInfo:errorDetail];
                completion(error);
                return nil;
            }

            break;
            
        default:
            [errorDetail setValue:@"Must pass valid action" forKey:NSLocalizedDescriptionKey];
            NSError *error = [NSError errorWithDomain:@"ParticleAPIError" code:0 userInfo:errorDetail];
            completion(error);
            return nil;

            break;
    }
    
    NSMutableDictionary *params = [[NSMutableDictionary alloc] init];
    if (actionString) {
        params[@"action"] = actionString;
    }
    if (mb_limit) {
        params[@"mb_limit"] = mb_limit;
    }
    if ([actionString isEqualToString:@"activate"]) {
        if (countryCode) {
            params[@"country"] = [countryCode uppercaseString];
        } else {
            params[@"country"] = @"US";
        }
    }

    if (cardToken) {
        params[@"card_token"] = cardToken;
    }



    NSString *url = [NSString stringWithFormat:@"/v1/sims/%@", iccid];

    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"PUT %@, params = %@", url, params];

    NSURLSessionDataTask *task = [self.manager PUT:url parameters:params headers:nil success:^(NSURLSessionDataTask * _Nonnull task, id _Nullable responseObject)
                                  {
                                      [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", url, (int)((NSHTTPURLResponse *)task.response).statusCode];
                                      [ParticleLogger logDebug:NSStringFromClass([self class]) format:@"%@", responseObject];

                                      NSDictionary *responseDict = responseObject;
                                      if (completion) {
                                          completion(nil);
                                      }
                                  } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error)
                                  {
                                      NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];
                                      
                                      if (completion) {
                                          // Handle 504s gracefully in client code
                                          completion(particleError);
                                      }

                                      [ParticleLogger logError:NSStringFromClass([self class]) format:@"! updateSim (%@) Failed %@ (%ld): %@\r\n%@", actionString, task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
                                  }];
    
    [self.manager.requestSerializer clearAuthorizationHeader];
    
    return task;
}







-(NSURLSessionDataTask *)_takeNetworkAction:(NSString *)action
                                   deviceID:(NSString *)deviceID
                                  networkID:(NSString *)networkID
                                 completion:(nullable ParticleCompletionBlock)completion
{
    if (self.session.accessToken) {
        NSString *authorization = [NSString stringWithFormat:@"Bearer %@",self.session.accessToken];
        [self.manager.requestSerializer setValue:authorization forHTTPHeaderField:@"Authorization"];
    }

    [ParticleLogger logError:NSStringFromClass([self class]) format:@"url: %@, action: %@, deviceId: %@", [NSString stringWithFormat:@"/v1/networks/%@", networkID], action, deviceID];

    NSMutableDictionary *params = [@{
            @"action": action,
            @"deviceID": deviceID,
    } mutableCopy];

    NSString *url = [NSString stringWithFormat:@"/v1/networks/%@", networkID];

    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"PUT %@, params = %@", url, params];

    NSURLSessionDataTask *task = [self.manager PUT:url parameters:params headers:nil success:^(NSURLSessionDataTask * _Nonnull task, id _Nullable responseObject)
    {
        [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", url, (int)((NSHTTPURLResponse *)task.response).statusCode];
        [ParticleLogger logDebug:NSStringFromClass([self class]) format:@"%@", responseObject];

        NSDictionary *responseDict = responseObject;
        if (completion) {
            if ([responseDict[@"ok"] boolValue])
            {
                completion(nil);
            }
            else
            {
                NSString *errorString;
                if (responseDict[@"errors"][0])
                    errorString = [NSString stringWithFormat:@"Could not modify network: %@",responseDict[@"errors"][0]];
                else
                    errorString = @"Error modifying network";

                NSError *particleError = [ParticleErrorHelper getParticleError:nil task:task customMessage:errorString];

                completion(particleError);

                [ParticleLogger logError:NSStringFromClass([self class]) format:@"! takeNetworkAction (%@) Failed %@ (%ld): %@\r\n%@", action, task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
            }
        }
    } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error)
    {
        NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];

        if (completion) {
            completion(particleError);
        }

        [ParticleLogger logError:NSStringFromClass([self class]) format:@"! takeNetworkAction (%@) Failed %@ (%ld): %@\r\n%@", action, task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
    }];

    [self.manager.requestSerializer clearAuthorizationHeader];

    return task;
}


-(NSURLSessionDataTask *)addDevice:(NSString *)deviceID toNetwork:(NSString *)networkID
                        completion:(nullable ParticleCompletionBlock)completion
{
    return [self _takeNetworkAction:@"add-device" deviceID:deviceID networkID:networkID completion:completion];

}

-(NSURLSessionDataTask *)removeDevice:(NSString *)deviceID fromNetwork:(NSString *)networkID
                           completion:(nullable ParticleCompletionBlock)completion
{
    return [self _takeNetworkAction:@"remove-device" deviceID:deviceID networkID:networkID completion:completion];

}

-(NSURLSessionDataTask *)enableGateway:(NSString *)deviceID onNetwork:(NSString *)networkID
                            completion:(nullable ParticleCompletionBlock)completion
{
    return [self _takeNetworkAction:@"gateway-enable" deviceID:deviceID networkID:networkID completion:completion];
}

-(NSURLSessionDataTask *)disableGateway:(NSString *)deviceID onNetwork:(NSString *)networkID
                             completion:(nullable ParticleCompletionBlock)completion
{
    return [self _takeNetworkAction:@"gateway-disable" deviceID:deviceID networkID:networkID completion:completion];

}


-(NSURLSessionDataTask *)removeDeviceNetworkInfo:(NSString *)deviceID
                           completion:(nullable ParticleCompletionBlock)completion
{
    if (self.session.accessToken) {
        NSString *authorization = [NSString stringWithFormat:@"Bearer %@",self.session.accessToken];
        [self.manager.requestSerializer setValue:authorization forHTTPHeaderField:@"Authorization"];
    }

    NSString *urlPath = [NSString stringWithFormat:@"/v1/devices/%@/network",deviceID];

    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"DELETE %@", urlPath];

    NSURLSessionDataTask *task = [self.manager DELETE:urlPath parameters:nil headers:nil success:^(NSURLSessionDataTask *task, id responseObject)
    {
        [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", urlPath, (int)((NSHTTPURLResponse *)task.response).statusCode];
        [ParticleLogger logDebug:NSStringFromClass([self class]) format:@"%@", responseObject];

        if (completion)
        {
            completion(nil);
        }
    } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error) {
        NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];

        if (completion)
        {
            completion( particleError);
        }

        [ParticleLogger logError:NSStringFromClass([self class]) format:@"! removeDeviceNetworkInfo Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
    }];

    return task;
}

-(NSString *)_actionToString:(ParticlePricingImpactAction)action {
    switch (action) {
        case ParticlePricingImpactActionAddUserDevice:
            return @"add-device-to-user";
        case ParticlePricingImpactActionAddNetworkDevice:
            return @"add-device-to-network";
        case ParticlePricingImpactActionCreateNetwork:
            return @"create-network";
    }
    return nil;
}

-(NSURLSessionDataTask *)getPricingImpact:(ParticlePricingImpactAction)action deviceID:(NSString * _Nullable)deviceID networkID:(NSString * _Nullable)networkID networkType:(ParticlePricingImpactNetworkType)networkType iccid:(NSString * _Nullable)iccid completion:(nullable void(^)(ParticlePricingInfo* _Nullable response, NSError * _Nullable))completion;
{
    if (self.session.accessToken) {
        NSString *authorization = [NSString stringWithFormat:@"Bearer %@",self.session.accessToken];
        [self.manager.requestSerializer setValue:authorization forHTTPHeaderField:@"Authorization"];
    }
    
    NSString *actionString;
    
    NSURL *url = [self.baseURL URLByAppendingPathComponent:@"v1/pricing-impact"];
    NSMutableDictionary *params = [[NSMutableDictionary alloc] init];

    params[@"action"] = [self _actionToString:action];
    if (deviceID) {
        params[@"device_id"] = deviceID;
    }
    if (networkID) {
        params[@"network_id"] = deviceID;
    }

    switch (networkType) {
        case ParticlePricingImpactNetworkTypeWifi:
            params[@"plan"] = @"wifi";
            break;
        case ParticlePricingImpactNetworkTypeCellular:
            params[@"plan"] = @"cellular";
            break;
    }

    if (iccid) {
        params[@"iccid"] = iccid;
    }

    [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"GET %@, params = %@", url.absoluteString, params];

    NSURLSessionDataTask *task = [self.manager GET:[url description] parameters:params headers:nil progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject)
                                  {
                                      [ParticleLogger logInfo:NSStringFromClass([self class]) format:@"%@ (%i)", url.absoluteString, (int)((NSHTTPURLResponse *)task.response).statusCode];
                                      [ParticleLogger logDebug:NSStringFromClass([self class]) format:@"%@", responseObject];

                                      if (completion)
                                      {
                                          NSHTTPURLResponse *serverResponse = (NSHTTPURLResponse *)task.response;
                                          NSDictionary *responseDict = responseObject;
                                          completion([[ParticlePricingInfo alloc] initWithParams:responseDict], nil);
                                      }
                                  } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error) {
                                      NSError *particleError = [ParticleErrorHelper getParticleError:error task:task customMessage:nil];
                                      
                                      if (completion)
                                      {
                                          completion(nil, particleError);
                                      }

                                      [ParticleLogger logError:NSStringFromClass([self class]) format:@"! getPricingImpact Failed %@ (%ld): %@\r\n%@", task.originalRequest.URL, (long)particleError.code, particleError.localizedDescription, particleError.userInfo[ParticleSDKErrorResponseBodyKey]];
                                  }];
    
    return task;
}





-(void)dealloc {
    [self unsubscribeToDevicesSystemEvents];
}

@end

NS_ASSUME_NONNULL_END
