//
//  HwManager.swift
//  Patriot
//
//  The HwManager has app lifetime so is held by the appFactory.
//  It can be accessed via the AppFactory
//
//  Created by Ron Lisle on 5/4/17.
//  Copyright © 2017 Ron Lisle. All rights reserved.
//

import Foundation


protocol HwManager: class
{
    var deviceDelegate:         DeviceNotifying?    { get set }
    var activityDelegate:       ActivityNotifying?  { get set }
    var eventName:              String              { get }
    var devices:                [DeviceInfo]        { get }
    var activities:             [ActivityInfo]      { get }
    
    func sendCommand(activity: String, isActive: Bool, completion: @escaping (Error?) -> Void)
    func sendCommand(device: String, percent: Int, completion: @escaping (Error?) -> Void)
}
