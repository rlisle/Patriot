//
//  DeviceInfo.swift
//  Patriot
//
//  DeviceInfo contains information read from a photon's Devices variable
//  It is passed to DeviceManager to create a Device object
//
//  Created by Ron Lisle on 6/2/18.
//  Copyright © 2018 Rons iMac. All rights reserved.
//

import Foundation

struct DeviceInfo {
    var photonName: String
    var name: String
    var type: DeviceType
    var percent: Int
    var room: String
}

extension DeviceInfo: Hashable {
    static func == (lhs: DeviceInfo, rhs: DeviceInfo) -> Bool {
        return lhs.name == rhs.name && lhs.type == rhs.type
    }

    func hash(into hasher: inout Hasher) {
        hasher.combine(name)
        hasher.combine(type)
    }
}
