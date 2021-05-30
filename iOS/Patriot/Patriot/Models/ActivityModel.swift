//
//  ActivityModel.swift
//  rvcp
//
//  Created by Ron Lisle on 10/7/16.
//  Copyright © 2016 Ron Lisle. All rights reserved.
//

import UIKit


class Activity
{
    let name:       String
    var onImage:    UIImage
    var offImage:   UIImage
    var isActive:   Bool
    
    init(name: String, isActive: Bool = false) {
        self.name     = name
        self.isActive = isActive
        self.onImage  = #imageLiteral(resourceName: "LightOn")
        self.offImage = #imageLiteral(resourceName: "LightOff")
    }
}
