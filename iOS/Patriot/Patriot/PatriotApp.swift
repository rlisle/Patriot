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
    
    @UIApplicationDelegateAdaptor(AppDelegate.self) var appDelegate
    @Environment(\.scenePhase) private var scenePhase
    @StateObject var model = PatriotModel.shared
    
    var body: some Scene {
        WindowGroup {
            HomeView()
                .environmentObject(model)
        }
        // We don't need to request authorization if custom intents only
//        .onChange(of: scenePhase) { phase in
//            INPreferences.requestSiriAuthorization({ status in
//                // Handle errors here
//            })
//        }
    }
}
