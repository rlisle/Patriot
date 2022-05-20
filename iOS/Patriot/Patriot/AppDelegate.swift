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

    // This shouldn't be needed if plist is specifying UISceneDelegateClassName, but takes priority if so
    // plist doesn't appear to be working, so will use this instead since it is working
    func application(_ application: UIApplication, configurationForConnecting connectingSceneSession: UISceneSession, options: UIScene.ConnectionOptions) -> UISceneConfiguration {

        print("AppDelegate connectingSceneSessions")

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
            return CurtainsHandler(application: application)    // TODO:
        case is PatriotIntent:
            print("Returning PatriotHandler")
            return PatriotHandler(application: application)    // TODO:
        default:
            print("Unhandled intent")
            return nil
        }
    }
    
    func application(_ application: UIApplication, continue userActivity: NSUserActivity, restorationHandler: @escaping ([UIUserActivityRestoring]?) -> Void) -> Bool {

        print("AppDelegate continue restorationHandler")

        guard userActivity.activityType == "WakingUp" else {
            return false
        }

//        guard let window = window,
//            let rootViewController = window.rootViewController as? HomeView else {
//                return false
//        }

//        restorationHandler([rootViewController])
        return true
    }
}
