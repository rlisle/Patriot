//
//  DevicesDataManager.swift
//  Patriot
//
//  Created by Ron Lisle on 5/31/18.
//  Copyright © 2018 Ron Lisle. All rights reserved.
//

import UIKit

class DevicesDataManager
{
    var devices:        [ Device ] = []
    let hardware:       HwManager
    weak var delegate:  DeviceNotifying?
    
    init(hardware: HwManager)
    {
        print("DevicesDataManager init")
        self.hardware = hardware
        
        devices.append(Device(name: "office", percent: 0))
        
        refresh(devices: hardware.devices)
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

    
    func setDevice(at: Int, percent: Int)
    {
        print("DM set device at: \(at) to \(percent)")
        devices[at].percent = percent
        let name = devices[at].name
//        if mqtt.isConnected {
//            mqtt.sendCommand(device: name, percent: percent)
//        } else {
            hardware.sendCommand(device: name, percent: percent) { (error) in
                if let error = error {
                    print("Send command error: \(error)")
                }
            }
//        }
    }
}


//MARK: Helper Methods

extension DevicesDataManager
{
    func refresh(devices: [DeviceInfo])
    {
        self.devices = []
        for device in devices
        {
            let name = device.name
            let percent = device.percent
            self.devices.append(Device(name: name, percent: percent))
        }
        delegate?.deviceListChanged()
    }
}


extension DevicesDataManager: DeviceNotifying
{
    func deviceListChanged()
    {
        print("DevicesDataManager deviceListChanged")
        let list = hardware.devices
        refresh(devices: list)
    }


    func deviceChanged(name: String, percent: Int)
    {
        print("DeviceDataManager: DeviceChanged: \(name)")
        if let index = devices.firstIndex(where: {$0.name == name})
        {
            print("   index of device = \(index)")
            devices[index].percent = percent
        }
        delegate?.deviceChanged(name: name, percent: percent)
    }
}
