//
//  ActivityInfo.swift
//  Patriot
//
//  Created by Ron Lisle on 6/2/18.
//  Copyright © 2018 Rons iMac. All rights reserved.
//

import Foundation

struct ActivityInfo {
    var name: String
    var isActive: Bool
}

extension ActivityInfo: Equatable {
    static func == (lhs: ActivityInfo, rhs: ActivityInfo) -> Bool {
        return lhs.name == rhs.name
    }
}
