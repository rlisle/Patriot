//
// Created by Raimundas Sakalauskas on 22/06/2018.
//

#import "KeychainHelper.h"
#import "ParticleLogger.h"


@implementation KeychainHelper

#pragma mark - Keychain Access
+ (NSString *)keychainValueForKey:(NSString *)key
{
    NSString *value = nil;

    NSMutableDictionary *query = [NSMutableDictionary dictionary];
    [query setObject:(__bridge id)(kSecClassGenericPassword) forKey:(__bridge id<NSCopying>)(kSecClass)];
    [query setObject:key forKey:(__bridge id<NSCopying>)(kSecAttrAccount)];
    [query setObject:(__bridge id)(kSecMatchLimitOne) forKey:(__bridge id<NSCopying>)(kSecMatchLimit)];
    [query setObject:(id)kCFBooleanTrue forKey:(__bridge id<NSCopying>)(kSecReturnData)];
    CFDictionaryRef cfresult = NULL;
    OSStatus err = SecItemCopyMatching((__bridge CFDictionaryRef)query, (CFTypeRef *)&cfresult);

    NSData *data = (__bridge_transfer NSData *)cfresult;
    if (err == noErr) {
        value = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    } else if (err != errSecItemNotFound) {
        [ParticleLogger logError:NSStringFromClass([self class]) format:@"Error reading keychain value = %qi", (long long)err];
    }
    return value;
}

+ (void)resetKeychainValueForKey:(NSString *)key {
    NSMutableDictionary *query = [NSMutableDictionary dictionary];
    [query setObject:(__bridge id)(kSecClassGenericPassword) forKey:(__bridge id<NSCopying>)(kSecClass)];
    [query setObject:key forKey:(__bridge id<NSCopying>)(kSecAttrAccount)];
    SecItemDelete((__bridge CFDictionaryRef)query);
}

+ (void)setKeychainValue:(NSString *)value forKey:(NSString *)key
{
    //remove existing value
    NSMutableDictionary *query = [NSMutableDictionary dictionary];
    [query setObject:(__bridge id)(kSecClassGenericPassword) forKey:(__bridge id<NSCopying>)(kSecClass)];
    [query setObject:key forKey:(__bridge id<NSCopying>)(kSecAttrAccount)];
    SecItemDelete((__bridge CFDictionaryRef)query);

    //add new value if one is provided
    if (value) {
        [query setObject:(__bridge id)kSecAttrAccessibleAfterFirstUnlock forKey:(__bridge id)kSecAttrAccessible];
        [query setObject:[value dataUsingEncoding:NSUTF8StringEncoding] forKey:(__bridge id<NSCopying>)(kSecValueData)];
        OSStatus err = SecItemAdd((__bridge CFDictionaryRef)query, nil);
        if (err != noErr) {
            [ParticleLogger logError:NSStringFromClass([self class]) format:@"Error adding keychain value = %qi", (long long)err];
        }
    }
}

@end
