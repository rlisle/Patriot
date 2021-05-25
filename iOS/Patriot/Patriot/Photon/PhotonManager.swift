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
//  The current activity state will be gleaned from the exposed Activities
//  properties of one or more Photons initially, but then tracked directly
//  after initialization by subscribing to particle or MQTT events.
//  TODO: convert to using the value() function
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
    var deviceDelegate:    DeviceNotifying?     // Reports changes to devices
    var activityDelegate:  ActivityNotifying?   // Reports changes to activities

    var isLoggedIn = false
    
    var photons: [String: Photon] = [: ]   // All the particle devices attached to logged-in user's account
    let eventName          = "patriot"
    
    //TODO: make these calculated properties using aggregation of photons collection
    var devices: [DeviceInfo] = []
    var activities:  [ActivityInfo] = []
}

extension PhotonManager: LoggingIn
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
                    self.getAllPhotonDevices()
                    completion(nil)
                    
                } else {
                    print ("Error logging in: \(error!)")
                    self.isLoggedIn = false
                    completion(error)
                }
            }
        }
    }
    
    func logout()
    {
        ParticleCloud.sharedInstance().logout()
        isLoggedIn = false
    }
}


extension PhotonManager: HwManager
{
    /**
     * Locate all the particle.io devices
     * This is an asynchronous process.
     * The delegates will be called as things are discovered.
     */
    func getAllPhotonDevices()
    {
        ParticleCloud.sharedInstance().getDevices {
            (photons: [ParticleDevice]?, error: Error?) in
            
            guard photons != nil && error == nil else {
                print("getAllPhotonDevices error: \(error!)")
                return
            }
            self.addAllPhotonsToCollection(photonDevices: photons!)
        }
    }


    func addAllPhotonsToCollection(photonDevices: [ParticleDevice])
    {
        self.photons = [: ]
        for photonDevice in photonDevices
        {
            if isValidPhoton(photonDevice)
            {
                if let name = photonDevice.name?.lowercased()
                {
                    let photon = Photon(device: photonDevice)
                    photon.delegate = self
                    self.photons[name] = photon
                    photon.refresh()
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
        let photon = photons[lowerCaseName]
        
        return photon
    }

    func sendCommand(activity: String, isActive: Bool, completion: @escaping (Error?) -> Void)
    {
        let event = activity + ":" + (isActive ? "100" : "0")
        publish(event: event, completion: completion)
    }

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
                    if let eventData = event?.data {
                        let splitArray = eventData.components(separatedBy: ":")
                        let name = splitArray[0].lowercased()
                        if let percent: Int = Int(splitArray[1]), percent >= 0, percent <= 100
                        {
                            //TODO: Currently can't tell if this is an activity or device
                            self.activityDelegate?.activityChanged(name: name, isActive: percent != 0)
                            self.deviceDelegate?.deviceChanged(name: name, percent: percent)
                        }
                        else
                        {
                            print("Event data is not a valid number")
                        }
                    }
                    
                })
            }
        })
    }
}


// These methods receive the capabilities of each photon asynchronously
extension PhotonManager: PhotonNotifying
{
    func device(named: String, hasDevices: [DeviceInfo])
    {
        for device in hasDevices {
            if device.name != "" && devices.contains(device) == false {
                devices.append(device)
            }
        }
        deviceDelegate?.deviceListChanged()
    }
    
    func device(named: String, hasActivities: [ActivityInfo])
    {
        for activity in hasActivities {
            if activity.name != "" && activities.contains(activity) == false {
                activities.append(activity)
            }
        }
        activityDelegate?.activitiesChanged()
    }
}


extension PhotonManager
{
    func readVariable(device: ParticleDevice, name: String, completion: @escaping (Any?, Error?) -> Void)
    {
        device.getVariable("Supported")
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
