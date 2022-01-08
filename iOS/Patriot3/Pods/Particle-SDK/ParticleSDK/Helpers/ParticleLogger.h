//
//  ParticleLogger.h
//  ParticleSDK
//
//  Created by Raimundas Sakalauskas on 27/11/2018.
//  Copyright © 2018 Particle Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

FOUNDATION_EXPORT NSString * const ParticleLogNotification;
FOUNDATION_EXPORT NSString * const ParticleLogNotificationTypeKey;
FOUNDATION_EXPORT NSString * const ParticleLogNotificationMessageKey;

typedef NS_ENUM(NSInteger, ParticleLogType) {
    ParticleLogTypeError=0,
    ParticleLogTypeInfo=1,
    ParticleLogTypeDebug=2,
    ParticleLogTypeComplete=3
};

typedef NS_ENUM(NSInteger, ParticleLoggerLevel) {
    ParticleLoggerLevelOff=-1,
    ParticleLoggerLevelError=0,
    ParticleLoggerLevelInfo=1,
    ParticleLoggerLevelDebug=2,
    ParticleLoggerLevelComplete=3
};


@interface ParticleLogger : NSObject

/**
 * Set desired level of logging.
 *
 * @param level all logs above set level are ignored.
 */
+ (void)setLoggerLevel:(ParticleLoggerLevel)level;

/**
 * Set desired level of logging for a control.
 *
 * @param level level all logs above set level are ignored.
 * @param control   control to apply selected level of logging to.
 */
+ (void)setLoggerLevel:(ParticleLoggerLevel)level forControl:(NSString *)control;

/**
 * Set controls that should be silenced.
 *
 * @param list string list of controls that should output no logs.
 */
+ (void)setIgnoreControls:(NSArray<NSString *> *)list;

/**
 * Base method for logging a message.
 *
 * @param control   name of the control outputting the message
 * @param type  type of the message
 * @param format    string format followed by args
 */
+ (void)log:(NSString *)control type:(ParticleLogType)type format:(NSString *)format, ...;

/**
 * Base method for logging a message.
 *
 * @param control   name of the control outputting the message
 * @param type  type of the message
 * @param format    string format
 * @param args  VAList args
 */
+ (void)log:(NSString *)control type:(ParticleLogType)type format:(NSString *)format withParameters:(va_list)args;

/**
 * Log message of error type.
 *
 * @param control   name of the control outputting the message
 * @param format    string format followed by args
 */
+ (void)logError:(NSString *)control format:(NSString *)format, ...;

/**
 * Log message of error type.
 *
 * @param control   name of the control outputting the message
 * @param format    string format
 * @param args  VAList args
 */
+ (void)logError:(NSString *)control format:(NSString *)format withParameters:(va_list)args;


/**
 * Log message of info type.
 *
 * @param control   name of the control outputting the message
 * @param type  type of the message
 * @param format    string format followed by args
 */
+ (void)logInfo:(NSString *)control format:(NSString *)format, ...;

/**
 * Log message of info type.
 *
 * @param control   name of the control outputting the message
 * @param type  type of the message
 * @param format    string format followed
 * @param args  VAList args
 */

+ (void)logInfo:(NSString *)control format:(NSString *)format withParameters:(va_list)args;

/**
 * Log message of debug type.
 *
 * @param control   name of the control outputting the message
 * @param format    string format followed by args
 */
+ (void)logDebug:(NSString *)control format:(NSString *)format, ...;

/**
 * Log message of debug type.
 *
 * @param control   name of the control outputting the message
 * @param type  type of the message
 * @param format    string format
 * @param args  VAList args
 */
+ (void)logDebug:(NSString *)control format:(NSString *)format withParameters:(va_list)args;

/**
 * Log message of complete type.
 *
 * @param control   name of the control outputting the message
 * @param type  type of the message
 * @param format    string format followed by args
 */
+ (void)logComplete:(NSString *)control format:(NSString *)format, ...;

/**
 * Log message of complete type.
 *
 * @param control   name of the control outputting the message
 * @param type  type of the message
 * @param format    string format followed
 * @param args  VAList args
 */
+ (void)logComplete:(NSString *)control format:(NSString *)format withParameters:(va_list)args;

/**
 * Helper function to convert ParticleLogType to NSString
 *
 * @param type ParticleLogType that should be converted to NSString
 * @return
 */
+ (NSString *)logTypeStringFromType:(ParticleLogType)type;

/**
 * Helper function to convert Int representing one of the ParticleLogTypes to NSString
 *
 * @param typeInt Int representing one of the ParticleLogTypes that should be converted to NSString
 * @return
 */
+ (NSString *)logTypeStringFromInt:(int)typeInt;

@end

NS_ASSUME_NONNULL_END
