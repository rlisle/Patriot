//
//  PatriotApp.swift
//  Patriot
//
//  Created by Ron Lisle on 5/25/21.
//

import SwiftUI
import Intents

@main
struct PatriotApp: App {

    @Environment(\.scenePhase) private var scenePhase
    
//    @UIApplicationDelegateAdaptor(AppDelegate.self) var appDelegate
    
    @StateObject var model = PatriotModel()
    
    var body: some Scene {
        WindowGroup {
            HomeView()
                .environmentObject(model)
        }
        .onChange(of: scenePhase) { phase in
            INPreferences.requestSiriAuthorization({ status in
                // Handle errors here
            })
        }
    }
}
