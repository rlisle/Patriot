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

class Device: ObservableObject, Codable
{
    @Published var percent:     Int
    @Published var isFavorite:  Bool

    let name:           String       // Mixed case, spaces allowed
    var onImageName:    String
    var offImageName:   String
    var type:           DeviceType
    var room:           String       // Mixed case, spaces allowed

    weak var publisher: DevicePublishing?
    
    private enum CodingKeys: CodingKey {
        case percent, isFavorite, name, onImageName, offImageName, type, room
    }
    
    func encode(to encoder: Encoder) throws {
        var container = encoder.container(keyedBy: CodingKeys.self)
        try container.encode(percent, forKey: .percent)
        try container.encode(isFavorite, forKey: .isFavorite)
        try container.encode(name, forKey: .name)
        try container.encode(onImageName, forKey: .onImageName)
        try container.encode(offImageName, forKey: .offImageName)
        try container.encode(type, forKey: .type)
        try container.encode(room, forKey: .room)
    }

    required init(from decoder: Decoder) throws {
        let container = try decoder.container(keyedBy: CodingKeys.self)
        percent = try container.decode(Int.self, forKey: .percent)
        isFavorite = try container.decode(Bool.self, forKey: .isFavorite)
        name = try container.decode(String.self, forKey: .name)
        onImageName = try container.decode(String.self, forKey: .onImageName)
        offImageName = try container.decode(String.self, forKey: .offImageName)
        type = try container.decode(DeviceType.self, forKey: .type)
        room = try container.decode(String.self, forKey: .room)
    }
    
    public init(name: String, type: DeviceType, percent: Int = 0, room: String = "Default", isFavorite: Bool = false) {
        self.name        = name
        self.type        = type
        self.percent     = percent
        self.room        = room
        self.isFavorite  = isFavorite
        self.publisher    = nil
        
        switch type {
        case .Curtain:
            self.onImageName = "CurtainOpen"
            self.offImageName = "CurtainClosed"
        case .Switch:
            self.onImageName = "SwitchOn"
            self.offImageName = "SwitchOff"
        default:
            self.onImageName = "LightOn"
            self.offImageName = "LightOff"
        }
    }
    
    func manualToggle() {
        manualSet(percent: percent == 0 ? 100 : 0)
    }
    
    func manualSet(percent: Int) {
        self.percent = percent
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
