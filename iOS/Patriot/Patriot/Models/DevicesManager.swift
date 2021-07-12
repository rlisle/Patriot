//
//  DevicesManager.swift
//  Patriot
//
//  This is the top level shared data repository
//
//  It will be passed to views via the environment
//  Uses an observed object instead of delegates
//
//  Created by Ron Lisle on 5/31/18, updated 7/10/21
//  Copyright © 2018-2021 Ron Lisle. All rights reserved.
//

import SwiftUI
import Combine

class DevicesManager: ObservableObject
{
    @Published var devices: [ Device ] = []
    @Published var favoritesList:  [String]                // List of favorite device names
    @Published var needsLogIn: Bool = true
    
    let photonManager:  PhotonManager
    let mqtt:           MQTTManager
    let settings:       Settings
    
    var favorites: [ Device ] {
        return devices.filter { $0.isFavorite == true }
    }

    init()
    {
        print("DevicesManager init")
        photonManager = PhotonManager()
        mqtt = MQTTManager()
        settings = Settings(store: UserDefaultsSettingsStore())
        favoritesList = settings.favorites ?? []
        mqtt.deviceDelegate = self          // Receives MQTT messages
        photonManager.deviceDelegate = self // Receives particle.io messages
        performAutoLogin()                  // During login all photons & devices will be created
    }

    // For Test/Preview
    convenience init(devices: [Device]) {
        self.init()
        self.devices = devices
    }

    func login(user: String, password: String) {
        photonManager.login(user: user, password: password) { error in
            guard error == nil else {
                self.needsLogIn = true
                print("Error auto logging in: \(error!)")
                return
            }
            self.needsLogIn = false
            // TODO: loading indicator or better yet progressive updates?
            self.photonManager.getAllPhotonDevices { deviceInfos, error in
                if error == nil {
                    self.addDeviceInfos(deviceInfos)
                }
            }
        }
    }
    
    func logout() {
        photonManager.logout()
        self.devices = []
        self.needsLogIn = true
    }
    
    func performAutoLogin() {
        if let user = settings.particleUser, let password = settings.particlePassword {
            login(user: user, password: password)
        }
    }
    
    func isDeviceOn(at: Int) -> Bool
    {
        return devices[at].percent > 0
    }

    
    func toggleDevice(at: Int)
    {
        let isOn = isDeviceOn(at: at)
        print("toggleDevice to \(isOn ? 0 : 100)")
        setDevice(at: at, percent: isOn ? 0 : 100)
    }

    func toggleFavorite(at: Int)
    {
        if let index = favoriteDeviceIndex(at: at) {
            toggleDevice(at: index)
        } else {
            print("favorite device not found for index \(at) == \(String(describing: index))")
        }
    }

    //TODO: Instead of index use hash (name+type)
    func favoriteDeviceIndex(at: Int) -> Int? {
        guard at < favorites.count else {
            return nil
        }
        let device = favorites[at]
        return indexOfDevice(device)
    }

    func indexOfDevice(_ device: Device) -> Int? {
        return devices.firstIndex(of: device) // { $0 == device }
    }
    
    func setDevice(at: Int, percent: Int)
    {
        print("DevicesManager set device at: \(at) to \(percent)")
        devices[at].percent = percent
        let name = devices[at].name
        if mqtt.isConnected {
            mqtt.sendPatriotMessage(device: name, percent: percent)
        } else {
            photonManager.sendCommand(device: name, percent: percent) { (error) in
                if let error = error {
                    print("Send command error: \(error)")
                }
            }
        }
    }
}

//MARK: Helper Methods

extension DevicesManager
{
    func addDeviceInfos(_ deviceInfos: [DeviceInfo])
    {
        print("DevicesManager addDeviceInfos")
//        self.devices = []   // Is this correct? Multiple Photons would clobber each other
        for deviceInfo in deviceInfos
        {
            let name = deviceInfo.name
            let type = deviceInfo.type
            let percent = deviceInfo.percent
            let newDevice = Device(name: name, type: type)
            newDevice.percent = percent
            newDevice.isFavorite = favoritesList.contains(name)
            self.devices.append(newDevice)
        }
//        delegate?.deviceListChanged()
    }
    
    //TODO: use hash
    func updateFavoritesList() {
        favoritesList = []
        for device in devices
        {
            if device.isFavorite {
                favoritesList.append(device.name)
            }
        }
        settings.favorites = favoritesList
    }
}


extension DevicesManager: DeviceNotifying
{
    func deviceChanged(name: String, percent: Int)
    {
        print("DeviceManager: DeviceChanged: \(name)")
        if let index = devices.firstIndex(where: {$0.name == name})
        {
            print("   index of device = \(index)")
            devices[index].percent = percent
        }
//        delegate?.deviceChanged(name: name, percent: percent)
    }
}
