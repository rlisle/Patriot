//
//  ContentView.swift
//  Patriot
//
//  Created by Ron Lisle on 5/25/21.
//

import SwiftUI

struct ContentView: View {

    @EnvironmentObject var devices: DevicesManager
    
    let columns = [
        GridItem(.adaptive(minimum: 80))
    ]

    var body: some View {
        ZStack {
            VStack {
                HStack {
                    Spacer()
                    Button("Log Out") {
                        devices.logout()
                    }
                    .foregroundColor(.white)
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

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
            .environmentObject(DevicesManager(
                                devices: [
                                    Device(name: "Light", type: .Light),
                                    Device(name: "Switch", type: .Switch),
                                    Device(name: "Curtain", type: .Curtain)
                                ]))
    }
}
