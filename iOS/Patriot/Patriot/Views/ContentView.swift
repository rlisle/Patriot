//
//  ContentView.swift
//  Patriot
//
//  Created by Ron Lisle on 5/25/21.
//

import SwiftUI

struct ContentView: View {

//    @EnvironmentObject var devices: DevicesManager
    
    var body: some View {
        GeometryReader { geometry in
            MainView()
                .frame(width: geometry.size.width, height: geometry.size.height)
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
