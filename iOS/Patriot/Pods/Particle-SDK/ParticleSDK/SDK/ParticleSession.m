//
//  ParticleSession.m
//  Particle iOS Cloud SDK
//
//  Created by Ido Kleinman.
//  Copyright (c) 2015-6 Particle. All rights reserved.
//

#import "ParticleSession.h"
#import "ParticleCloud.h"

NS_ASSUME_NONNULL_BEGIN

NSString *const kParticleSessionKeychainEntry = @"io.particle.api.Keychain.AccessToken";
NSString *const kParticleSessionExpiryDateKey = @"kParticleSessionExpiryDateKey";
NSString *const kParticleSessionAccessTokenStringKey = @"kParticleSessionAccessTokenStringKey";
NSString *const kParticleSessionRefreshTokenStringKey = @"kParticleSessionRefreshTokenStringKey";
NSString *const kParticleSessionUsernameStringKey = @"kParticleSessionUsernameStringKey";


// how many seconds before expiry date will a token be considered expired (0 = expire on expiry date, 24*60*60 = expire a day before)
#define ACCESS_TOKEN_EXPIRY_MARGIN  0

@interface ParticleSession()

@property (nonatomic, strong) NSDate *expiryDate;
@property (nonatomic, strong) NSTimer *expiryTimer;
@property (nonatomic, strong, nullable, readwrite) NSString *accessToken;
@property (nonatomic, nullable, strong, readwrite) NSString *refreshToken;
@property (nonatomic, strong, nullable, readwrite) NSString *username;

@end

@implementation ParticleSession

-(nullable instancetype)initWithNewSession:(NSDictionary *)loginResponseDict
{
    self = [super init];
    if (self)
    {
//        NSLog(@"(debug)login responseObject:\n%@",loginResponseDict.description);
        NSNumber *nti = loginResponseDict[@"expires_in"];
        if (!nti) return nil;
        
        if ([nti integerValue]==0)
            self.expiryDate = [NSDate distantFuture];
        else
            self.expiryDate = [[NSDate alloc] initWithTimeIntervalSinceNow:nti.doubleValue];

        self.accessToken = loginResponseDict[@"access_token"];
        if (!self.accessToken)
            return nil;
        
        self.refreshToken = loginResponseDict[@"refresh_token"];
        if (!self.refreshToken)
            return nil;
        
        self.username = loginResponseDict[@"username"]; //injected to dict by SDK
        
        // verify response object type
        if (![loginResponseDict[@"token_type"] isEqualToString:@"bearer"])
            return nil;

        [self storeSessionInKeychainAndSetExpiryTimer];
        
        return self;
    }
    
    return nil;
}


-(nullable instancetype)initWithToken:(NSString *)token
{
    self = [super init];
    if (self)
    {
        if (!token)
            return nil;
        
        self.accessToken = token;
        self.expiryDate = [NSDate distantFuture];
        self.refreshToken = nil;

        [self storeSessionInKeychainAndSetExpiryTimer];
        
        return self;
    }
    
    return nil;
}

-(void)storeSessionInKeychainAndSetExpiryTimer
{
    if (![self.expiryDate isEqualToDate:[NSDate distantFuture]]) {
        self.expiryTimer = [[NSTimer alloc] initWithFireDate:self.expiryDate interval:0 target:self selector:@selector(accessTokenExpired:) userInfo:nil repeats:NO];
        [[NSRunLoop currentRunLoop] addTimer:self.expiryTimer forMode:NSDefaultRunLoopMode];
    }
    
    NSMutableDictionary *accessTokenDict = [NSMutableDictionary new];
    accessTokenDict[kParticleSessionAccessTokenStringKey] = self.accessToken;
    accessTokenDict[kParticleSessionExpiryDateKey] = self.expiryDate;
    if (self.refreshToken)
        accessTokenDict[kParticleSessionRefreshTokenStringKey] = self.refreshToken;
    if (self.username)
        accessTokenDict[kParticleSessionUsernameStringKey] = self.username;
    

    //prepare data to be stored into keychain
    NSMutableDictionary *jsonDict = [accessTokenDict mutableCopy];
    //convert expiry date to NSNumber for easier serialization
    jsonDict[kParticleSessionExpiryDateKey] = @(((NSDate *)jsonDict[kParticleSessionExpiryDateKey]).timeIntervalSince1970);

    NSError *err;
    NSData *jsonData = [NSJSONSerialization  dataWithJSONObject:jsonDict options:0 error:&err];
    NSString *keychainString = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
    [KeychainHelper setKeychainValue:keychainString forKey:kParticleSessionKeychainEntry];
}



-(nullable instancetype)initWithToken:(NSString *)token andExpiryDate:(NSDate *)expiryDate
{
    self = [super init];
    if (self)
    {
        if (!expiryDate)
            return nil;
        
        if (!token)
            return nil;
        
        self.expiryDate = expiryDate;
        self.accessToken = token;
        self.refreshToken = nil;
        self.username = nil;
        
        [self storeSessionInKeychainAndSetExpiryTimer];
        return self;
    }
    
    return nil;
}

-(nullable instancetype)initWithToken:(NSString *)token withExpiryDate:(NSDate *)expiryDate withRefreshToken:(NSString *)refreshToken
{
    self = [super init];
    if (self)
    {
        if (!expiryDate)
            return nil;
        
        if (!token)
            return nil;
        
        if (!refreshToken)
            return nil;
        
        self.expiryDate = expiryDate;
        self.accessToken = token;
        self.refreshToken = refreshToken;
        self.username = nil;
        
        [self storeSessionInKeychainAndSetExpiryTimer];
        return self;
    }
    
    return nil;
}


-(nullable instancetype)initWithSavedSession
{
    self = [super init];
    if (self)
    {

        NSString *keychainEntry = [KeychainHelper keychainValueForKey:kParticleSessionKeychainEntry];
        NSDictionary *accessTokenDict;

        if (keychainEntry != nil && keychainEntry.length > 0) {
            NSData *data =[keychainEntry dataUsingEncoding:NSUTF8StringEncoding];
            NSError * err;
            accessTokenDict = (NSDictionary *)[NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableContainers error:&err];

            if (err != nil){
                [self removeSession];
            }
        } else {
            return nil;
        }

        
        if (accessTokenDict)
        {
            self.accessToken = accessTokenDict[kParticleSessionAccessTokenStringKey];
            if ([accessTokenDict objectForKey:kParticleSessionExpiryDateKey]) {
                //expiration date is serialized as NSTimeInterval since 1970
                self.expiryDate = [[NSDate alloc] initWithTimeIntervalSince1970:[accessTokenDict[kParticleSessionExpiryDateKey] doubleValue]];
            }
            if ([accessTokenDict objectForKey:kParticleSessionRefreshTokenStringKey]) {
                self.refreshToken = accessTokenDict[kParticleSessionRefreshTokenStringKey];
            } else {
                self.refreshToken = nil;
            }
            if ([accessTokenDict objectForKey:kParticleSessionUsernameStringKey]) {
                self.username = accessTokenDict[kParticleSessionUsernameStringKey];
            } else {
                self.username = nil;
            }
            
        }
        else
            return nil;

        // this also checks if saved session access token has expired already (by getter)
        if (!((self.accessToken) && (self.expiryDate)))
            return nil;
        
        if (![self.expiryDate isEqualToDate:[NSDate distantFuture]]) {
            self.expiryTimer = [[NSTimer alloc] initWithFireDate:self.expiryDate interval:0 target:self selector:@selector(accessTokenExpired:) userInfo:nil repeats:NO];
            [[NSRunLoop currentRunLoop] addTimer:self.expiryTimer forMode:NSDefaultRunLoopMode];
        }
        
        return self;
    }
    
    return nil;
}


-(nullable NSString *)accessToken
{
    // always return only a non-expired access token
    if (!self.expiryDate)
        return _accessToken;
    
    NSTimeInterval ti = [self.expiryDate timeIntervalSinceNow];
    if (ti < ACCESS_TOKEN_EXPIRY_MARGIN)
        return nil;
    else
        return _accessToken;
}


-(void)removeSession
{
    [KeychainHelper resetKeychainValueForKey:kParticleSessionKeychainEntry];
    self.accessToken = nil;
    self.username = nil;
    self.refreshToken = nil;
    self.expiryDate = [NSDate distantFuture];
}

-(void)accessTokenExpired:(NSTimer *)timer
{
    [self.expiryTimer invalidate];
    [self.delegate ParticleSession:self didExpireAt:self.expiryDate];
}

-(void)dealloc
{
    [self.expiryTimer invalidate];
}

@end

NS_ASSUME_NONNULL_END
