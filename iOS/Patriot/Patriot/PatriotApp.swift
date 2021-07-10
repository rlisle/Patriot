//
//  PatriotApp.swift
//  Patriot
//
//  Created by Ron Lisle on 5/25/21.
//

import SwiftUI

@main
struct PatriotApp: App {

    @StateObject var devices = DevicesManager()
    
    var body: some Scene {
        WindowGroup {
            ContentView()
                .environmentObject(devices)
        }
    }
}
