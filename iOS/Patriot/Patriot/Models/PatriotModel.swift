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
    @Published var showingLogin: Bool = false
    
    let photonManager:  PhotonManager
    let mqtt:           MQTTManager
    let settings:       Settings
    
    var favorites: [ Device ] {
        return devices.filter { $0.isFavorite == true }
    }

    // List of actually named rooms (ignores All, Default, etc)
    var rooms: [ String ] {
        let rawRooms = devices.map { $0.room }.filter { $0 != "All" && $0 != "Default" && $0 != "Test" }
        let uniqueRooms = Set<String>(rawRooms)
        return Array(uniqueRooms).sorted()
    }
    
    init(forTest: Bool = false)
    {
        photonManager = PhotonManager()
        mqtt = MQTTManager()
        settings = Settings(store: UserDefaultsSettingsStore())
        favoritesList = settings.favorites ?? []
        mqtt.mqttDelegate = self          // Receives MQTT messages
        photonManager.particleIoDelegate = self // Receives particle.io messages
        if forTest {
            devices = getTestDevices()
        }
    }

    // For Test/Preview
    convenience init(devices: [Device]) {
        self.init(forTest: true)
        self.devices = Array(Set(devices))
    }
}


// LogIn
extension PatriotModel {
    
    func login(user: String, password: String) {
        photonManager.login(user: user, password: password) { error in
            guard error == nil else {
                self.showingLogin = true
                print("Error auto logging in: \(error!)")
                return
            }
            self.showingLogin = false
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
        self.showingLogin = true
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
        guard isConnected == true else {
            print("Connected disconnected")
            return
        }
        print("MQTT is connected")
        // This needs to be done here so subscribe is already set
        mqtt.sendMessage(topic: "patriot/query", message: "all")
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
    // New state format: "patriot/state/room/<t>/name value"
    func didReceiveMessage(topic: String, message: String) {
        // Parse out known messages
        let splitTopic = topic.components(separatedBy: "/")
        let percent: Int = Int(message) ?? -1
        let command = splitTopic[1]
        switch (command, splitTopic.count) {
            
        case (_, 2):
            if let device = devices.first(where: {$0.name.lowercased() == command.lowercased()}) {
                device.percent = percent
            }
            
        case ("state", 5):
            let room = splitTopic[2]
            let type = splitTopic[3].uppercased()
            let deviceName = splitTopic[4]
            if let device = getDevice(room: room, device: deviceName) {
                // Set existing device value
                device.percent = percent
                //print("Setting device \(room):\(deviceName) to \(percent)")
            } else if isDisplayableDevice(type: type) {
                // create new device
                //print("Creating device \(room):\(deviceName) with value \(percent)")
                addDevice(Device(name: deviceName, type: DeviceType(rawValue: type) ?? .Light, percent: percent, room: room, isFavorite: false))
            }

        default:
            print("Message unrecognized or deprecated: \(topic), \(message)")
        }
    }
    
    func isDisplayableDevice(type: String) -> Bool {
        return type == "C" || type == "F" || type == "L";
    }
    
    func getDevice(room: String, device: String) -> Device? {
        return devices.first(where: {$0.name.lowercased() == device.lowercased() && $0.room.lowercased() == room.lowercased() })
    }
}


// Favorites
extension PatriotModel {

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
        //print("PatriotModel addDeviceInfos, count: \(deviceInfos.count)")
        var accumulatedDevices: Set<Device> = Set<Device>(devices)
        for deviceInfo in deviceInfos
        {
            let newDevice = Device(deviceInfo)
            newDevice.publisher = self
            newDevice.isFavorite = favoritesList.contains(deviceInfo.name)
            accumulatedDevices.insert(newDevice)
        }
        self.devices = Array(accumulatedDevices)
        //print("Devices.count now \(devices.count)")
    }
    
    func addDevice(_ device: Device)
    {
        //print("PatriotModel addDevice \(device.name)")
        devices.append(device)
        device.publisher = self
        device.isFavorite = favoritesList.contains(device.name)
    }
}

// Called when MQTT or particle.io device message received
extension PatriotModel: DeviceNotifying
{
    func deviceChanged(name: String, percent: Int)
    {
        //print("DeviceManager: DeviceChanged: \(name)")
        if let index = devices.firstIndex(where: {$0.name == name})
        {
            //print("   index of device = \(index)")
            devices[index].percent = percent
        }
    }
}

// Called by a device when manually changed
extension PatriotModel: DevicePublishing {
    func devicePercentChanged(device: Device) {
        //print("PatriotModel: devicePercentChanged: \(device.name), \(device.type) = \(device.percent)")
        sendMessage(device: device)
    }

    func isFavoriteChanged(device: Device) {
        updateFavoritesList()
    }
}

// Test Devices
extension PatriotModel {
    func getTestDevices() -> [Device] {
        return [
            Device(name: "Light1", type: .Light, percent: 0, room: "Office", isFavorite: true),
            Device(name: "Switch1", type: .Switch, percent: 0, room: "Office", isFavorite: false),
            Device(name: "Curtain1", type: .Curtain, percent: 0, room: "Office", isFavorite: true),
            Device(name: "Light2", type: .Light, percent: 100, room: "Kitchen"),
            Device(name: "Switch2", type: .Switch, percent: 0, room: "Kitchen"),
            Device(name: "Light3", type: .Light, percent: 100, room: "Kitchen"),
            Device(name: "Light4", type: .Light, percent: 0, room: "Living Room"),
            Device(name: "Light5", type: .Light, percent: 0, room: "Living Room"),
            Device(name: "Light6", type: .Light, percent: 0, room: "Living Room"),
            Device(name: "Light7", type: .Light, percent: 0, room: "Living Room"),
            Device(name: "Light8", type: .Light, percent: 0, room: "Living Room"),
            Device(name: "Light9", type: .Light, percent: 0, room: "Living Room"),
            Device(name: "Light10", type: .Light, percent: 0, room: "Living Room"),
            Device(name: "Light11", type: .Light, percent: 0, room: "Living Room"),
            Device(name: "Light12", type: .Light, percent: 0, room: "Bedroom"),
            Device(name: "Light13", type: .Light, percent: 0, room: "Bedroom"),
            Device(name: "Switch14", type: .Switch, percent: 0, room: "Living Room"),
            Device(name: "Light15", type: .Light, percent: 0, room: "Kitchen"),
            Device(name: "Relay16", type: .Relay, percent: 0, room: "Bedroom"),
            Device(name: "Light17", type: .Light, percent: 100, room: "Living Room"),
            Device(name: "Light18", type: .Light, percent: 0, room: "Living Room")
        ]
    }
}
