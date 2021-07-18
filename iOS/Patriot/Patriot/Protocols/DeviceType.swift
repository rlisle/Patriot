//
//  DeviceType.swift
//  Patriot
//
//  Created by Ron Lisle on 6/2/18.
//  Copyright © 2018 Rons iMac. All rights reserved.
//

import Foundation

enum DeviceType: String {
    case Unknown = "?"
    case Curtain = "C"
    case Fan = "F"
    case Light = "L"
    case Motor = "M"
    case NCD8Relay = "N"  //placeholder
    case PartOfDay = "P"
    case Proximity = "X"
    case Relay = "R"
    case Switch = "S"
    case TempHumidity = "T"
    case Ultrasonic = "U"
}
