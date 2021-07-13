//
//  MainView.swift
//  Patriot
//
//  Created by Ron Lisle on 7/13/21.
//

import SwiftUI

struct MainView: View {
    
    @EnvironmentObject var devices: DevicesManager

    let columns = [
        GridItem(.adaptive(minimum: 80))
    ]

    var body: some View {
        ZStack {
            VStack {
                HStack {
                    Button("Log Out") {
                        devices.logout()
                    }
                    .foregroundColor(.white)
                    Spacer()
                }
                .padding()
                ScrollView {
                    LazyVGrid(columns: columns, spacing: 20) {
                        ForEach(devices.devices, id: \.self) { device in
                            DeviceView(device: device)
                        }
                    }
                    .padding(.horizontal)
                }
            }
            .padding(.top, 32)
        }
        .background(Color(.black))
        .edgesIgnoringSafeArea(.all)
//        .frame(maxHeight: 300)
        .sheet(isPresented: $devices.needsLogIn) {
            LoginView()
        }
    }
}

struct MainView_Previews: PreviewProvider {
    static var previews: some View {
        MainView()
            .environmentObject(DevicesManager(
                                devices: [
                                    Device(name: "Light", type: .Light),
                                    Device(name: "Switch", type: .Switch),
                                    Device(name: "Curtain", type: .Curtain)
                                ]))
    }
}
