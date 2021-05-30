//
//  ParticleEvent.h
//  Pods
//
//  Created by Ido on 7/14/15.
//
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class ParticleEvent;

typedef void (^ParticleEventHandler)(ParticleEvent * _Nullable event, NSError * _Nullable error);

@interface ParticleEvent : NSObject

/**
 * Event published by this device ID
 */
@property (nonatomic, strong) NSString *deviceID;

/**
 * Event payload in string format
 */
@property (nonatomic, nullable, strong) NSString *data;

/**
 * Event name
 */
@property (nonatomic, strong) NSString *event;

/**
 * Event "published at" time/date UTC
 */
@property (nonatomic, strong) NSDate *time;

/**
 * Event time to live (currently unused)
 */
@property (nonatomic) NSInteger ttl;

/**
 *  Particle event handler class initializer which receives a dictionary argument
 *
 *  @param eventDict NSDictionary argument which contains the event payload keys: event (name), data (payload), ttl (time to live), published_at (date/time published), coreid (publishiing device ID).
 */
-(instancetype)initWithEventDict:(NSDictionary *)eventDict;

@end

NS_ASSUME_NONNULL_END
