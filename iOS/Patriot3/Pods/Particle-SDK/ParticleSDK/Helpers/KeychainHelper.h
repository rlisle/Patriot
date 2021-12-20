//
// Created by Raimundas Sakalauskas on 22/06/2018.
//

#import <Foundation/Foundation.h>


/// Simplified interface for KeyChain access.
@interface KeychainHelper : NSObject


/// Get string representation of particular keychain entry.
///
/// @param key keychain entry key.
+ (NSString *)keychainValueForKey:(NSString * _Nonnull)key;


/// Remove particular keychain entry.
///
/// @param key keychain entry key.
+ (void)resetKeychainValueForKey:(NSString * _Nonnull)key;


/// Set keychain entry using key and value
///
/// @param value keychain entry value.
/// @param key keychain entry key.
+ (void)setKeychainValue:(NSString * _Nullable)value forKey:(NSString * _Nonnull)key;

@end