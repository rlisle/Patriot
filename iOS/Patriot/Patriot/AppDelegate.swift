//
//  AppDelegate.swift
//  Patriot
//
//  This is needed to handle intents
//
//  Created by Ron Lisle on 5/15/22.
//

import SwiftUI
import Intents
import IntentsUI

class AppDelegate: NSObject, UIApplicationDelegate {
    // Just to confirm this is working
    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey : Any]? = nil) -> Bool {
        print("applicationDidFinishLaunching")
        return true
    }
    
    func application(_ application: UIApplication, handlerFor intent: INIntent) -> Any? {
        print("AppDelegate handlerFor")
        switch intent {
        case is WakingUpIntent:
            print("Returning WakingUpHandler")
            return WakingUpHandler(application: application)
        case is RetiringIntent:
            print("Returning RetiringHandler")
            return WakingUpHandler(application: application)
        case is SleepingIntent:
            print("Returning SleepingHandler")
            return WakingUpHandler(application: application)
        default:
            print("Unhandled intent")
            return nil
        }
    }
}

class WakingUpHandler: NSObject, WakingUpIntentHandling {
    let application: UIApplication
    
    init(application: UIApplication) {
        self.application = application
    }
    
    func handle(intent: WakingUpIntent,
        completion: @escaping (WakingUpIntentResponse) -> Void) {
        
        print("WakingUpHandler")
        
            if application.applicationState == .background {
                completion(WakingUpIntentResponse(code: .continueInApp, userActivity: nil))
            } else {
                
                // Update UI
                
                completion(WakingUpIntentResponse(code: .success, userActivity: nil))
            }
        
    }
}
