//
//  Device.swift
//  Patriot
//
//  Created by Ron Lisle on 5/31/18.
//  Copyright © 2018 Ron Lisle. All rights reserved.
//

import SwiftUI

protocol DevicePublishing: AnyObject {
    func devicePercentChanged(device: Device)
    func isFavoriteChanged(device: Device)
}

class Device: ObservableObject
{
    @Published var percent:     Int
    @Published var isFavorite:  Bool

    let name:      String
    var onImage:   UIImage
    var offImage:  UIImage
    var type:      DeviceType

    weak var publisher: DevicePublishing?
    
    public init(name: String, type: DeviceType, percent: Int = 0, isFavorite: Bool = false) {
        self.name        = name
        self.type        = type
        self.percent     = percent
        self.isFavorite  = isFavorite
        self.publisher    = nil
        
        switch type {
        case .Curtain:
            self.onImage = #imageLiteral(resourceName: "CurtainOpen")
            self.offImage = #imageLiteral(resourceName: "CurtainClosed")
        case .Switch:
            self.onImage = #imageLiteral(resourceName: "SwitchOn")
            self.offImage = #imageLiteral(resourceName: "SwitchOff")
        default:
            self.onImage = #imageLiteral(resourceName: "LightOn")
            self.offImage = #imageLiteral(resourceName: "LightOff")
        }
    }
    
    func manualToggle() {
        percent = percent == 0 ? 100 : 0
        publisher?.devicePercentChanged(device: self)
    }
    
    func flipFavorite() {
        isFavorite = !isFavorite
        publisher?.isFavoriteChanged(device: self)
    }
}

extension Device: Hashable {
    
    static func == (lhs: Device, rhs: Device) -> Bool {
        return lhs.name == rhs.name && lhs.type == rhs.type
    }
    
    func hash(into hasher: inout Hasher) {
        hasher.combine(name)
        hasher.combine(type)
    }
}
