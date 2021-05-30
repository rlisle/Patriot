//
//  DeviceModel.swift
//  Patriot
//
//  Created by Ron Lisle on 5/31/18.
//  Copyright © 2018 Ron Lisle. All rights reserved.
//

import UIKit


class Device
{
    let name:       String
    var onImage:    UIImage
    var offImage:   UIImage
    var percent:    Int
    
    init(name: String, percent: Int = 0) {
        self.name    = name
        self.percent = percent
        self.onImage = #imageLiteral(resourceName: "LightOn")
        self.offImage = #imageLiteral(resourceName: "LightOff")
    }
}
