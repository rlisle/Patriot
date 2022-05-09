//
//  SectionView.swift
//  Patriot
//
//  Created by Ron Lisle on 7/18/21.
//

import SwiftUI

struct SectionView: View {
    
    @State var title: String
    @State var devices: [Device]
    
    let columns = [
        GridItem(.adaptive(minimum: 80, maximum: 160))
    ]

    var body: some View {
        Section(
            header:
                HStack {
                    Spacer()
                    Text(title)
                    Spacer()
                }
                .foregroundColor(.white)
                .background(Color(.gray))
            ) {
                LazyVGrid(columns: columns, spacing: 20) {
                    ForEach(devices, id: \.self) { device in
                        DeviceView(device: device)
                            .aspectRatio(1, contentMode: .fill)
                            .padding()
                    }
                }
            }
        .padding(.horizontal)
        //.background(Color(.black).ignoresSafeArea())
    }
}

struct SectionView_Previews: PreviewProvider {
    static var previews: some View {
        SectionView(title: "Living Room",
                    devices: [
                        Device(name: "Light1", type: .Light, percent: 0, room: "Living Room", isFavorite: true),
                    Device(name: "Switch1", type: .Switch, room: "Living Room"),
                    Device(name: "Curtain1", type: .Curtain),
                    Device(name: "Light2", type: .Light, percent: 100, room: "Living Room"),
                    Device(name: "Switch2", type: .Switch),
                    Device(name: "Light3", type: .Light)
                    ]
            )
    }
}
