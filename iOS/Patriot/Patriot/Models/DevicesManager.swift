//
//  DevicesManager.swift
//  Patriot
//
//  Created by Ron Lisle on 5/31/18.
//  Copyright © 2018 Ron Lisle. All rights reserved.
//

import UIKit

class DevicesManager
{
    var devices:        [ Device ] = []
    let photonManager:  PhotonManager
    let mqtt:           MQTTManager
    let settings:       Settings
    var favoritesList:  [String]                // List of favorite device names
    weak var delegate:  DeviceNotifying?

    var favorites: [ Device ] {
        return devices.filter { $0.isFavorite == true }
    }

    init(photonManager: PhotonManager, mqtt: MQTTManager, settings: Settings)
    {
        print("DevicesManager init")
        self.photonManager = photonManager
        self.mqtt = mqtt
        self.settings = settings
        favoritesList = settings.favorites ?? []
        mqtt.deviceDelegate = self
        refresh(devices: photonManager.devices)
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
    func refresh(devices: [DeviceInfo])
    {
        self.devices = []
        for device in devices
        {
            let name = device.name
            let type = device.type
            let percent = device.percent
            let newDevice = Device(name: name, type: type)
            newDevice.percent = percent
            newDevice.isFavorite = favoritesList.contains(name)
            self.devices.append(newDevice)
        }
        delegate?.deviceListChanged()
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
    func deviceListChanged()
    {
        print("DevicesManager deviceListChanged")
        let list = photonManager.devices
        refresh(devices: list)
    }


    func deviceChanged(name: String, percent: Int)
    {
        print("DeviceManager: DeviceChanged: \(name)")
        if let index = devices.firstIndex(where: {$0.name == name})
        {
            print("   index of device = \(index)")
            devices[index].percent = percent
        }
        delegate?.deviceChanged(name: name, percent: percent)
    }
}
