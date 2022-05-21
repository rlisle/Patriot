//
//  SettingsStore.swift
//  Patriot
//
//  This class implements SettingsStore using UserDefaults
//
//  Created by Ron Lisle on 5/24/18.
//  Copyright © 2018 Rons iMac. All rights reserved.
//

import Foundation

// We'll use the default string rawValue for the key in the store
enum SettingsKey: String
{
    case particleUser
    case particlePassword
    case favorites
    case devices
}

protocol SettingsStore
{
    func getBool(forKey: SettingsKey) -> Bool?
    func set(_ bool: Bool?, forKey: SettingsKey)
    func getInt(forKey: SettingsKey) -> Int?
    func set(_ int: Int?, forKey: SettingsKey)
    func getString(forKey: SettingsKey) -> String?
    func set(_ string: String?, forKey: SettingsKey)
    func getStringArray(forKey: SettingsKey) -> [String]?
    func set(_ array: [String]?, forKey: SettingsKey)
    func getDeviceArray(forKey: SettingsKey) -> [Device]?
    func set(_ array: [Device]?, forKey: SettingsKey)
}

class UserDefaultsSettingsStore: SettingsStore
{
    let userDefaults = UserDefaults.standard
    
    func getBool(forKey key: SettingsKey) -> Bool?
    {
        return userDefaults.bool(forKey: key.rawValue)
    }
    
    
    func set(_ bool: Bool?, forKey key: SettingsKey)
    {
        userDefaults.set(bool, forKey: key.rawValue)
    }
    
    
    func getInt(forKey key: SettingsKey) -> Int?
    {
        return userDefaults.integer(forKey: key.rawValue)
    }
    
    
    func set(_ int: Int?, forKey key: SettingsKey)
    {
        userDefaults.set(int, forKey: key.rawValue)
    }
    
    
    func getString(forKey key: SettingsKey) -> String?
    {
        return userDefaults.string(forKey: key.rawValue)
    }
    
    
    func set(_ string: String?, forKey key: SettingsKey)
    {
        userDefaults.set(string, forKey: key.rawValue)
    }
    
    func getStringArray(forKey key: SettingsKey) -> [String]?
    {
        return userDefaults.array(forKey: key.rawValue) as? [String]
    }
    
    
    func set(_ stringArray: [String]?, forKey key: SettingsKey)
    {
        userDefaults.set(stringArray, forKey: key.rawValue)
    }
    
    func getDeviceArray(forKey key: SettingsKey) -> [Device]?
    {
        return userDefaults.array(forKey: key.rawValue) as? [Device]
    }
    
    
    func set(_ deviceArray: [Device]?, forKey key: SettingsKey)
    {
        userDefaults.set(deviceArray, forKey: key.rawValue)
    }
}

class Settings
{
    let store: SettingsStore
    
    init(store: SettingsStore)
    {
        self.store = store
    }
}


extension Settings
{
    var particleUser: String?
    {
        get {
            return store.getString(forKey: .particleUser)
        }
        set {
            store.set(newValue, forKey: .particleUser)
        }
    }
    
    
    var particlePassword: String? {
        get {
            return store.getString(forKey: .particlePassword)
        }
        set {
            store.set(newValue, forKey: .particlePassword)
        }
    }

    var favorites: [String]? {
        get {
            return store.getStringArray(forKey: .favorites)
        }
        set {
            store.set(newValue, forKey: .favorites)
        }
    }
    
    var devices: [Device]? {
        get {
            return store.getDeviceArray(forKey: .devices)
        }
        set {
            store.set(newValue, forKey: .devices)
        }
    }

}
