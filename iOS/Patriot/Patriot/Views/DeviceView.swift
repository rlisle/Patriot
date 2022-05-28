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

    @EnvironmentObject var model: PatriotModel
    @ObservedObject var device: Device
    @State var brighten = false

    var body: some View {

        GeometryReader { geometry in
            ZStack {
                VStack {
                    Image(device.percent > 0 ? device.onImageName : device.offImageName)
                    .resizable()
                    .scaledToFit()
                    .padding(0)
                    .opacity(brighten ? 1.0 : 0.8)
                    .onTapGesture {
                        device.manualToggle()
                    }
                    .onLongPressGesture(minimumDuration: 1.0, perform: {
                        model.selectedDevice = self.device
                        model.showingDetails = true
                    }, onPressingChanged: { pressed in
                        brighten = pressed
                    })
                    Spacer()
                }
                VStack {
                    HStack {
                        Spacer()
                        Button(action: device.flipFavorite) {
                            Image(systemName: device.isFavorite ? "star.fill" : "star")
                                .renderingMode(.template)
                                .resizable()
                                .foregroundColor(device.isFavorite ? Color("HotPink") : Color("LightPink"))
                                .padding(4)
                                .frame(width: geometry.size.width/4, height: geometry.size.width/4)
                        }
                    }
                    Spacer()
                    Text(device.name.camelCaseToWords())
                        .font(.subheadline)
                        .foregroundColor(Color("LightPink"))
                }
            }
            .frame(width: geometry.size.width, height: geometry.size.width)
            .background(Color("Maroon"))
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
