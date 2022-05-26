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
    // Singleton
    static var shared = PatriotModel()

    // Convenience binding for sidemenu, details
    @Published var showingMenu = false
    @Published var showingDetails = false
    var selectedDevice = Device(name: "none", type: .Light)
    
    @Published var devices: [Device] = []
    @Published var favoritesList:  [String]   //TODO: delete & refactor using devices only             // List of favorite device names
    @Published var sleeping: Sleeping = .Awake
    @Published var partOfDay: PartOfDay = .Afternoon
    
    let mqtt:           MQTTManager
    let settings:       Settings
    
    var didQueryDevices = false
    var didReceiveDevices = false
    
    var alive: [ String : String ] = [:]      // eg. rearpanel : Sun 04:52
    var logs: [ String ] = []
    var loglevels: [ String : String ] = [:]  // eg. rearpanel : warn
    var memory: String = ""                   // TODO: provide controller + int
    var pings: [ String : Date ] = [:]
    var pongs: [ String : Date ] = [:]
    var queries: [ String : Date ] = [:]
    var resets: [ String : Date ] = [:]

    var rooms: [ String ] {
        let rawRooms = devices.map { $0.room }.filter { $0 != "All" && $0 != "Default" && $0 != "Test" }
        let uniqueRooms = Set<String>(rawRooms)
        return Array(uniqueRooms).sorted()
    }
    
    init(forTest: Bool = false,
         sleeping: Sleeping = .Awake,
         partOfDay: PartOfDay = .Afternoon
        )
    {
        settings = Settings(store: UserDefaultsSettingsStore())
        devices = settings.devices ?? []  // Initialize with cached devices
        mqtt = MQTTManager(forTest: forTest)
        favoritesList = settings.favorites ?? []
        mqtt.mqttDelegate = self          // Receives MQTT messages
        if forTest {
            devices = getTestDevices()
            self.sleeping = sleeping
            self.partOfDay = partOfDay
        }
    }

    // For Test/Preview
    convenience init(devices: [Device]) {
        self.init(forTest: true)
        self.devices = Array(Set(devices))
    }
}


// Device Messaging
extension PatriotModel: MQTTReceiving {
    
    func connectionDidChange(isConnected: Bool) {
        if isConnected == false {
            print("MQTT disconnected, reconnecting")
            mqtt.reconnect()
            
            if isConnected == false {
                return
            }
        }
        
        print("MQTT is connected")
        // This needs to be done here so subscribe is already set
        if didQueryDevices == false {
            didQueryDevices = true
            mqtt.sendMessage(topic: "patriot/query", message: "all")
        }
    }

    func sendMessage(device: Device)
    {
        //print("PatriotModel sendMessage \(device.name) to \(device.percent)")
        if mqtt.isConnected {
            mqtt.sendPatriotMessage(device: device.name, percent: device.percent)
        }
    }
    
    func sendMessage(topic: String, message: String)
    {
        print("PatriotModel sendMessage \(topic), \(message)")
        if mqtt.isConnected {
            print("Sending to MQTT")
            mqtt.sendMessage(topic: topic, message: message)
        } else {
            print("sendMessage \(topic), \(message) not sent. No connection.")
        }
    }
    
    // MQTT or Particle.io message received
    // New state format: "patriot/state/room/<t>/name value"
    func didReceiveMessage(topic: String, message: String) {
        
        //print("didReceiveMessage: \(topic), \(message)")
        
        // Parse out known messages
        let splitTopic = topic.components(separatedBy: "/")
        let percent: Int = Int(message) ?? -1
        let command = splitTopic[1]
        
        switch (command, splitTopic.count) {
            
        case (_, 1):                                    // Topic: patriot, Message: device:percent
            handleLegacyCommand(message: message)
            
        case (_, 2):
            handleCommand(name: command, percent:percent)
            
        case ("alive", 3):
            alive[ splitTopic[2] ] = message
            
        case ("log", 3):
            logs.append( splitTopic[2] + message )
            
        case ("loglevel", 3):
            loglevels[ splitTopic[2] ] = message
            
        case ("memory", 2):
            memory = message

        case ("ping", 2):
            pings[ message ] = Date()
            
        case ("pong", 2):
            pongs[ message ] = Date()

        case ("query", 2):
            queries[ message ] = Date()
            
        case ("reset", 2):
            resets[ message ] = Date()
            
        case ("state", 5):
            if didReceiveDevices == false {
                print("Receiving devices by MQTT")
                devices = []
                didReceiveDevices = true
            }
            let room = splitTopic[2]
            let type = splitTopic[3].uppercased()
            let deviceName = splitTopic[4]
            handleState(name: deviceName, room: room, type: type, percent: percent)

        default:
            print("Message unrecognized or deprecated: \(topic), \(message)")
        }
    }
    
    func isDisplayableDevice(type: String) -> Bool {
        //let displayableTypes = "CFLR"
        return /*displayableTypes.contains(type)*/ type == "C" || type == "F" || type == "L" || type == "R"
    }
}

extension PatriotModel {
    
    func handleState(name: String, room: String, type: String, percent: Int) {

        guard handleSpecialNames(name: name, percent: percent) == false else {
            return
        }
            
        if let device = getDevice(name: name, room: room) {
            // Set existing device value
            device.percent = percent
            //print("Setting device \(room):\(name) to \(percent)")
            
        } else if isDisplayableDevice(type: type) {
            // create new device
            //print("Creating device \(room):\(name) with value \(percent)")
            addDevice(Device(name: name, type: DeviceType(rawValue: type) ?? .Light, percent: percent, room: room, isFavorite: false))
        }
    }
    
    func handleLegacyCommand(message: String) {
        let splitMessage = message.components(separatedBy: ":")
        guard splitMessage.count == 2 else {
            print("Legacy command missing colon")
            return
        }
        let percent: Int = Int(splitMessage[1]) ?? -1
        let deviceName = splitMessage[0]
        guard handleSpecialNames(name: deviceName, percent: percent) == false else {
            return
        }

        if let device = getDevice(name: deviceName) {
            device.percent = percent
        }
    }
    
    func handleCommand(name: String, percent: Int) {
        guard handleSpecialNames(name: name, percent: percent) == false else {
            return
        }

        if let device = getDevice(name: name) {
            device.percent = percent
        }
    }
    
    func handleSpecialNames(name: String, percent: Int) -> Bool {
        
        if name.caseInsensitiveCompare("sleeping") == .orderedSame {
            print("Sleeping = \(percent)")
            self.sleeping = Sleeping(rawValue: percent) ?? .unknown
            return true
            
        } else if name.caseInsensitiveCompare("partofday") == .orderedSame {
            print("PartOfDay = \(percent)")
            self.partOfDay = PartOfDay(rawValue:percent) ?? .unknown
            return true

        }
        return false
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

// Devices
extension PatriotModel {
    
    func getDevice(name: String, room: String? = nil) -> Device? {
        if let room = room {
            return devices.first(where: { $0.name.caseInsensitiveCompare(name) == .orderedSame
                && $0.room.caseInsensitiveCompare(room) == .orderedSame })
        } else {
            return devices.first(where: { $0.name.caseInsensitiveCompare(name) == .orderedSame } )
        }
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
