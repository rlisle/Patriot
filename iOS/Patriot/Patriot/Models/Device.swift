//
//  Device.swift
//  Patriot
//
//  Created by Ron Lisle on 5/31/18.
//  Copyright © 2018 Ron Lisle. All rights reserved.
//

import UIKit

protocol DeviceDelegate: AnyObject {
    func devicePercentChanged(name: String, type: DeviceType, percent: Int)
    func isFavoriteChanged(name: String, type: DeviceType, isFavorite: Bool)
}

class Device
{
    let name:          String
    var onImage:       UIImage
    var offImage:      UIImage
    var type:          DeviceType
    private var _percent: Int
    private var _isFavorite: Bool

    weak var delegate: DeviceDelegate?

    var percent:       Int {
        get {
            return _percent
        }
        set {
            _percent = newValue
            delegate?.devicePercentChanged(name: name, type: type, percent: _percent)
        }
    }
    var isFavorite:    Bool {
        get {
            return _isFavorite
        }
        
        set {
            _isFavorite = newValue
            delegate?.isFavoriteChanged(name: name, type: type, isFavorite: _isFavorite)
        }
    }
    
    init(name: String, type: DeviceType, percent: Int = 0, isFavorite: Bool = false) {
        self.name        = name
        self.type        = type
        self._percent    = percent
        self._isFavorite = isFavorite
        self.delegate    = nil
        
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
    
    func toggle() {
        percent = _percent == 0 ? 100 : 0
    }
    
    func flipFavorite() {
        isFavorite = !isFavorite
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
