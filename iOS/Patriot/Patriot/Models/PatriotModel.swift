//
//  PatriotModel.swift
//  Patriot
//
//  This is the top level model
//
//  It will be passed to views via the environment
//  Is an observed object instead of using delegates for updating UI
//
//  Created by Ron Lisle on 5/31/18, updated 7/10/21
//  Copyright © 2018-2021 Ron Lisle. All rights reserved.
//

import SwiftUI
import Combine

class PatriotModel: ObservableObject
{
    @Published var devices: [Device] = []
    @Published var favoritesList:  [String]   //TODO: delete & refactor using devices only             // List of favorite device names
    @Published var needsLogIn: Bool = true
    
    let photonManager:  PhotonManager
    let mqtt:           MQTTManager
    let settings:       Settings
    
    var favorites: [ Device ] {
        return devices.filter { $0.isFavorite == true }
    }

    init()
    {
        print("PatriotModel init")
        photonManager = PhotonManager()
        mqtt = MQTTManager()
        settings = Settings(store: UserDefaultsSettingsStore())
        favoritesList = settings.favorites ?? []
        mqtt.mqttDelegate = self          // Receives MQTT messages
        photonManager.particleIoDelegate = self // Receives particle.io messages
        performAutoLogin()                  // During login all photons & devices will be created
    }

    // For Test/Preview
    convenience init(devices: [Device]) {
        self.init()
        self.devices = Array(Set(devices))
    }
}


// LogIn
extension PatriotModel {
    
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
                    print("Found \(deviceInfos.count) photon devices")
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
            print("Performing auto-login \(user), \(password)")
            login(user: user, password: password)
        }
    }
}

// Device Messaging
extension PatriotModel: MQTTReceiving {
    
    func connectionDidChange(isConnected: Bool) {
        print("Connection changed")
    }

    func sendMessage(device: Device)
    {
        print("PatriotModel sendMessage \(device.name) to \(device.percent)")
        if mqtt.isConnected {
            mqtt.sendPatriotMessage(device: device.name, percent: device.percent)
        } else {
            photonManager.sendCommand(device: device) { (error) in
                if let error = error {
                    print("sendMessage particle.io error: \(error)")
                }
            }
        }
    }
    
    // MQTT or Particle.io message received
    func didReceiveMessage(topic: String, message: String) {
        // Parse out device commands: "patriot/<devicename>"
        let splitTopic = topic.components(separatedBy: "/")
        guard splitTopic.count >= 2 else {
            print("Message invalid topic: \(topic)")
            return
        }
        
        let name = splitTopic[1].lowercased()
        if let percent: Int = Int(message), percent >= 0, percent <= 255
        {
            if let device = devices.first(where: {$0.name == name}) {
                device.percent = percent
            }
        }
        else
        {
            print("Event data is not a valid number: \(message)")
        }
    }
}


// Favorites
extension PatriotModel {

//    func isDeviceFavorite(at: Int) -> Bool
//    {
//        return devices[at].isFavorite
//    }
//
//    func toggleFavorite(at: Int)
//    {
//        if let index = favoriteDeviceIndex(at: at) {
//            setFavorite(at: index)
//        } else {
//            print("favorite device not found for index \(at) == \(String(describing: index))")
//        }
//    }
//
//    //TODO: Instead of index use hash (name+type)
//    func favoriteDeviceIndex(at: Int) -> Int? {
//        guard at < favorites.count else {
//            return nil
//        }
//        let device = favorites[at]
//        return indexOfDevice(device)
//    }
    
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

// Add
extension PatriotModel {
    
    func addDeviceInfos(_ deviceInfos: [DeviceInfo])
    {
        print("PatriotModel addDeviceInfos, count: \(deviceInfos.count)")
        var accumulatedDevices: Set<Device> = Set<Device>(devices)
        for deviceInfo in deviceInfos
        {
            let newDevice = Device(name: deviceInfo.name, type: deviceInfo.type)
            newDevice.publisher = self
            newDevice.percent = deviceInfo.percent
            newDevice.isFavorite = favoritesList.contains(deviceInfo.name)
            accumulatedDevices.insert(newDevice)
        }
        self.devices = Array(accumulatedDevices)
        print("Devices.count now \(devices.count)")
    }
}

// Called when MQTT or particle.io device message received
extension PatriotModel: DeviceNotifying
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

// Called by a device when manually changed
extension PatriotModel: DevicePublishing {
    func devicePercentChanged(device: Device) {
        print("PatriotModel: devicePercentChanged: \(device.name), \(device.type) = \(device.percent)")
        sendMessage(device: device)
    }

    func isFavoriteChanged(device: Device) {
        updateFavoritesList()
    }
}
