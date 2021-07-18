//
//  Sleeping.swift
//  Patriot
//
//  This file duplicates defines from IoT.h for use in Swift
//
//  Created by Ron Lisle on 7/17/21.
//

import Foundation

struct PartOfDay {
    static let Sunrise = 1
    static let Morning = 2
    static let Noon = 3
    static let Afternoon = 4
    static let Sunset = 5
    static let Dusk = 6
    static let Night = 7
    static let Dawn = 8
}

struct Sleeping {
    static let Awake = 1
    static let Retiring = 2
    static let Asleep = 3
}
