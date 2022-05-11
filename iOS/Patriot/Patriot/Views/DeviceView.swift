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
    
    @ObservedObject var device: Device

    var body: some View {
        GeometryReader { geometry in
            ZStack {
                Button(action: device.manualToggle) {
                    Image(uiImage: device.percent > 0 ? device.onImage : device.offImage)
                        .resizable()
                        .scaledToFit()
                        .padding(0)
                }
                VStack {
                    HStack {
                        Spacer()
                        Button(action: device.flipFavorite) {
                            Image(systemName: device.isFavorite ? "star.fill" : "star")
                                .renderingMode(.template)
                                .resizable()
                                .foregroundColor(device.isFavorite ? .yellow : .gray)
                                .padding(4)
                                .frame(width: geometry.size.width/4, height: geometry.size.width/4)
                        }
                    }
                    Spacer()
                    Text(device.name)
                        .font(.subheadline)
                        .foregroundColor(.white)
                }
            }
            .frame(width: geometry.size.width, height: geometry.size.width)
            .background(Color.black)
        }
    }
}

struct DeviceView_Previews: PreviewProvider {
    static var previews: some View {
        Group {
            DeviceView(device: Device(name: "Light", type: .Light, percent: 0, isFavorite: false))
                .previewLayout(PreviewLayout.fixed(width: 160, height: 160))
                .previewDisplayName("Device Off")
            
            DeviceView(device: Device(name: "Light", type: .Light, percent: 100, isFavorite: true))
                .previewLayout(PreviewLayout.fixed(width: 160, height: 160))
                .previewDisplayName("Device Favorite On")
        }
    }
}
