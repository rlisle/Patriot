//
//  HwController.swift
//  Patriot
//
//  This protocol represents each microcontroller
//  Currently these are Particle.io Photons, but they
//  might be something different in the future.

//  Created by Ron Lisle on 5/6/17.
//  Copyright © 2017 Ron Lisle. All rights reserved.
//

import Foundation
import Particle_SDK


protocol HwController
{
    var devices: [DeviceInfo]           { get }
    var activities: [ActivityInfo]      { get }
    var publish: String                 { get }
    var name: String                    { get }
    init(device: ParticleDevice)
    func refresh()
}
