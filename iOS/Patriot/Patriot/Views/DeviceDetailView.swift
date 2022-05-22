//
//  DeviceDetailView.swift
//  Patriot
//
//  Created by Ron Lisle on 5/22/22.
//

import SwiftUI

struct DeviceDetailView: View {
    
    let device: Device
    
    var body: some View {
        //TODO: add more stuff'
        VStack {
            Text("Room: \(device.room)")
            Text(device.name)
        }
    }
}

struct DeviceDetailView_Previews: PreviewProvider {
    static var previews: some View {
        DeviceDetailView(device: Device(name: "Test Device", type: .Light))
    }
}
