//
//  DeviceView.swift
//  Patriot
//
//  Displays a single device & state
//
//  Created by Ron Lisle on 7/10/21.
//

import SwiftUI

struct DeviceView: View {
    
    @State var device: Device

    var body: some View {
        ZStack {
            Image("LightOff")
                .resizable()
                .aspectRatio(contentMode: .fill)
            VStack {
                Spacer()
                Text(device.name)
                    .foregroundColor(.white)
            }
        }
    }
}

struct DeviceView_Previews: PreviewProvider {
    static var previews: some View {
        DeviceView(device: Device(name: "Light", type: .Light))
    }
}
