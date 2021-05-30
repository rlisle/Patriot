//
//  DeviceInfo.swift
//  Patriot
//
//  Created by Ron Lisle on 6/2/18.
//  Copyright © 2018 Rons iMac. All rights reserved.
//

import Foundation

struct DeviceInfo {
    var name: String
    var type: DeviceType
    var percent: Int
}

extension DeviceInfo: Equatable {
    static func == (lhs: DeviceInfo, rhs: DeviceInfo) -> Bool {
        return lhs.name == rhs.name
    }
}
