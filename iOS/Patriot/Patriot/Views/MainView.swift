//
//  MainView.swift
//  Patriot
//
//  Created by Ron Lisle on 7/13/21.
//

import SwiftUI

struct MainView: View {
    
    @EnvironmentObject var devices: PatriotModel

    @Binding var showMenu: Bool
    
    let columns = [
        GridItem(.adaptive(minimum: 80, maximum: 160))
    ]

    var body: some View {
        VStack {
            ScrollView {
                LazyVGrid(columns: columns, spacing: 20) {
                    ForEach(devices.devices, id: \.self) { device in
                        DeviceView(device: device)
                            .aspectRatio(1, contentMode: .fill)
                            .padding()
                    }
                }
                .padding(.horizontal)
            }
        }
        .padding(.top, 16)
        .background(Color(.black).ignoresSafeArea())
        .sheet(isPresented: $devices.needsLogIn) {
            LoginView()
        }
    }
}

struct MainView_Previews: PreviewProvider {
    static var previews: some View {
        StatefulPreviewWrapper(true) { MainView(showMenu: $0) }
            .environmentObject(PatriotModel(
                                devices: [
                                    Device(name: "Light", type: .Light, percent: 0, isFavorite: true),
                                    Device(name: "Switch", type: .Switch),
                                    Device(name: "Curtain", type: .Curtain),
                                    Device(name: "Light2", type: .Light, percent: 100),
                                    Device(name: "Switch2", type: .Switch),
                                    Device(name: "Light3", type: .Light),
                                    Device(name: "Curtain2", type: .Curtain),
                                    Device(name: "Light4", type: .Light)
                                ]))
    }
}
