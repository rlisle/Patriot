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
        ScrollView {
            LazyVGrid(columns: columns, spacing: 20) {
                ForEach(devices.devices, id: \.self) { device in
                    DeviceView(device: device)
                }
            }
            .padding(.horizontal)
        }
//        .frame(maxHeight: 300)
        .sheet(isPresented: $devices.NeedsLogIn) {
            LoginView()
        }
    }
            
//            Menu("Settings") {
//                Button("Log Out") {
//                    devices.NeedsLogIn = true
//                }
//            }
//        }
//        .sheet(isPresented: $devices.NeedsLogIn) {
//            LoginView()
//        }
//    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
            .environmentObject(DevicesManager())
    }
}
