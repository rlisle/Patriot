//
//  DeviceNotifying.swift
//  Patriot
//
//  Created by Ron Lisle on 12/10/16.
//  Copyright © 2016 Ron Lisle. All rights reserved.
//

import Foundation


protocol DeviceNotifying: AnyObject
{
    func deviceListChanged()
    func deviceChanged(name: String, percent: Int)
}
