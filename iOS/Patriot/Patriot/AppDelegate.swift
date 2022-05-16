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

class AppDelegate: NSObject, UIApplicationDelegate, ObservableObject {
    // Just to confirm this is working
    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey : Any]? = nil) -> Bool {
        print("applicationDidFinishLaunching")
        return true
    }
    
    // This shouldn't be needed if plist is specifying UISceneDelegateClassName, but takes priority if so
    func application(_ application: UIApplication, configurationForConnecting connectingSceneSession: UISceneSession, options: UIScene.ConnectionOptions) -> UISceneConfiguration {
    
        print("connectingSceneSessions")
        
        let sceneConfig = UISceneConfiguration(name: "SceneDelegate", sessionRole: connectingSceneSession.role)
        sceneConfig.delegateClass = SceneDelegate.self
        return sceneConfig
    }
    
    func application(_ application: UIApplication, handlerFor intent: INIntent) -> Any? {
        print("AppDelegate handlerFor")
        switch intent {
        case is WakingUpIntent:
            print("Returning WakingUpHandler")
            return WakingUpHandler(application: application)
        case is RetiringIntent:
            print("Returning RetiringHandler")
            return WakingUpHandler(application: application)    // TODO:
        case is SleepingIntent:
            print("Returning SleepingHandler")
            return WakingUpHandler(application: application)    // TODO:
        case is CurtainsIntent:
            print("Returning CurtainsHandler")
            return WakingUpHandler(application: application)    // TODO:
        default:
            print("Unhandled intent")
            return nil
        }
    }
    
    func application(_ application: UIApplication, continue userActivity: NSUserActivity, restorationHandler: @escaping ([UIUserActivityRestoring]?) -> Void) -> Bool {
        
        print("continue restorationHandler")
        
        guard userActivity.activityType == "WakingUp" else {
            return false
        }
        
//        guard let window = window,
//            let rootViewController = window.rootViewController as? SwitchViewController else {
//                return false
//        }
//
//        restorationHandler([rootViewController])
        return true
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
