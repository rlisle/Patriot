//
//  DeviceView.swift
//  Patriot
//
//  Displays a single device & state
//
//  Created by Ron Lisle on 7/10/21.
//

import SwiftUI

extension String {
    func camelCaseToWords() -> String {
        return unicodeScalars.dropFirst().reduce(String(prefix(1))) {
            return CharacterSet.uppercaseLetters.contains($1)
                ? $0 + " " + String($1)
                : $0 + String($1)
        }
    }
}

struct DeviceView: View {
    
    @ObservedObject var device: Device

    var body: some View {
        GeometryReader { geometry in
            ZStack {
                VStack {
                    Button(action: device.manualToggle) {
                        Image(uiImage: device.percent > 0 ? device.onImage : device.offImage)
                            .resizable()
                            .scaledToFit()
                            .padding(0)
                    }
                    Spacer()
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
                                //TODO: specify sizes smarter
                                .frame(width: geometry.size.width/4, height: geometry.size.width/4)
                        }
                    }
                    Spacer()
                    Text(device.name.camelCaseToWords())
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
            DeviceView(device: Device(name: "LeftTrim", type: .Light, percent: 0, isFavorite: false))
                .previewLayout(PreviewLayout.fixed(width: 100, height: 100))
                .previewDisplayName("Device Off")
            
            DeviceView(device: Device(name: "KitchenCeiling", type: .Light, percent: 100, isFavorite: true))
                .previewLayout(PreviewLayout.fixed(width: 100, height: 100))
                .previewDisplayName("Device Favorite On")
        }
    }
}
