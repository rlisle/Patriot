//
//  PhotonManager.swift
//  Patriot
//
//  This class manages the collection of Photon microcontrollers
//
//  Discovery will search for all the Photon devices on the network
//  in the logged-in user's account.
//
//  When a new device is found, it will be added to the photons collection
//  and a delegate called.
//
//  Subscribing to particle events will also allow detecting new Photons
//  as they come online.
//
//  This file uses the Particle SDK:
//      https://docs.particle.io/reference/ios/#common-tasks
//
//  Created by Ron Lisle on 11/13/16.
//  Copyright © 2016, 2017 Ron Lisle. All rights reserved.
//

import Foundation
import Particle_SDK

class PhotonManager: NSObject
{
    var subscribeHandler:  Any?                 // Particle.io subscribe handle
    private var isLoggedIn = false              // Is this needed?
    var photons: [String: Photon] = [: ]        // All the particle devices attached to logged-in user's account
    let eventName          = "patriot"
    var deviceDelegate: DeviceNotifying?        // Called by particle.io subscribe
}

extension PhotonManager
{
    /**
     * Login to the particle.io account
     * The particle SDK will use the returned token in subsequent calls.
     * We don't have to save it.
     */
    func login(user: String, password: String, completion: @escaping (Error?) -> Void)
    {
        if !isLoggedIn {
            
            ParticleCloud.sharedInstance().login(withUser: user, password: password) { (error) in
                if error == nil {
                    self.isLoggedIn = true
                    self.subscribeToEvents()
                } else {
                    self.isLoggedIn = false
                }
                completion(error)
            }
        }
    }
    
    func logout()
    {
        ParticleCloud.sharedInstance().logout()
        photons = [:]
        isLoggedIn = false
    }
    
    /**
     * Locate all the particle.io devices
     * This is an asynchronous process.
     * The completion will be called once for each photon discovered.
     */
    func getAllPhotonDevices(completion: @escaping ([DeviceInfo], Error?)-> Void)
    {
        ParticleCloud.sharedInstance().getDevices {
            (photons: [ParticleDevice]?, error: Error?) in
            
            guard photons != nil && error == nil else {
                print("getAllPhotonDevices error: \(error!)")
                completion([], error)
                return
            }
            self.addAllPhotonsToCollection(photonDevices: photons!, completion: completion)
        }
    }


    func addAllPhotonsToCollection(photonDevices: [ParticleDevice], completion: @escaping ([DeviceInfo], Error?)-> Void)
    {
        self.photons = [: ]
        for photonDevice in photonDevices
        {
            if isValidPhoton(photonDevice)
            {
                if let name = photonDevice.name?.lowercased()
                {
                    let photon = Photon(device: photonDevice)
                    self.photons[name] = photon
                    photon.readDevices() { deviceInfos in
                        completion(deviceInfos, nil)
                    }
                }
            }
        }
    }
    
    
    func isValidPhoton(_ device: ParticleDevice) -> Bool
    {
        return device.connected
    }
    
    
    func getPhoton(named: String) -> Photon?
    {
        let lowerCaseName = named.lowercased()
        return photons[lowerCaseName]
    }

    // Use MQTT instead
    func sendCommand(device: String, percent: Int, completion: @escaping (Error?) -> Void)
    {
        let event = device + ":" + String(percent)
        publish(event: event, completion: completion)
    }

    func publish(event: String, completion: @escaping (Error?) -> Void)
    {
        ParticleCloud.sharedInstance().publishEvent(withName: eventName, data: event, isPrivate: true, ttl: 60)
        { (error:Error?) in
            if let e = error
            {
                print("Error publishing event \(e.localizedDescription)")
            }
            completion(error)
        }
    }
    
    func subscribeToEvents()
    {
        subscribeHandler = ParticleCloud.sharedInstance().subscribeToMyDevicesEvents(withPrefix: eventName, handler: { (event: ParticleEvent?, error: Error?) in
            if let error = error {
                print("Error subscribing to events: \(error)")
            }
            else
            {
                DispatchQueue.main.async(execute: {
                    //TODO: convert to new format. event._event = patriot/<devicename>, event._data = message
                    guard let eventMessage = event?.data,
                          let eventTopic = event?.event else {
                        print("MQTT received event with missing data")
                        return
                    }
                        
                    let splitTopic = eventTopic.components(separatedBy: "/")
                    guard splitTopic.count >= 2 else {
                        print("Invalid topic: \(eventTopic)")
                        return
                    }
                    
                    let name = splitTopic[1].lowercased()
                    if let percent: Int = Int(eventMessage), percent >= 0, percent <= 255
                    {
                        self.deviceDelegate?.deviceChanged(name: name, percent: percent)
                    }
                    else
                    {
                        print("Particle.io event data is not a valid number: \(eventMessage)")
                    }
                })
            }
        })
    }
}


// These methods receive the capabilities of each photon asynchronously
//extension PhotonManager: PhotonDeviceInfoNotifying
//{
//    func photon(named: String, hasDeviceInfos: Set<DeviceInfo>)
//    {
//        for device in hasDeviceInfos {
//            if device.name != "" && devices.contains(device) == false {
//                devices.append(device)
//            }
//        }
//        deviceDelegate?.deviceListChanged()
//    }
//}


extension PhotonManager
{
    func readVariable(device: ParticleDevice, name: String, completion: @escaping (Any?, Error?) -> Void)
    {
        device.getVariable(name)
        { (result: Any?, error: Error?) in
            if let variable = result as? String
            {
                completion(variable, nil)
            }
            else
            {
                completion(nil, error!)
            }
        }
    }
}
