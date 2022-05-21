//
//  SceneDelegate.swift
//  Patriot
//
//  Created by Ron Lisle on 5/16/22.
//

import SwiftUI

class SceneDelegate: NSObject, UIWindowSceneDelegate, ObservableObject {
    
    var window: UIWindow?
    
    func sceneWillEnterForeground(_ scene: UIScene) {
        print("sceneWillEnterForeground")
    }
    
    func sceneDidBecomeActive(_ scene: UIScene) {
        print("sceneDidBecomeActive")
    }
    
    func sceneWillResignActive(_ scene: UIScene) {
        print("sceneWillResignActive")
    }
    
//    func scene(_ scene: UIScene, continue userActivity: NSUserActivity) {
//        print("scene continue userActivity")
//        handleUserActivity(userActivity)
//    }
//
//    func handleUserActivity(_ userActivity: NSUserActivity) {
//
//        print("Scene handleUserActivity")
//
//        guard let interaction = userActivity.interaction else {
//            print("Scene interaction not set")
//            return
//        }
//        guard let window = window else {
//            print("Scene window not set")
//            return
//        }
//        guard let rootViewController = window.rootViewController as? UINavigationController else {
//            print("Scene rootViewController not set")
//            return
//        }
//
//        print("Scene TODO: handle it")
//        if let intent = interaction.intent as? ShowDirectionsIntent,
//           let recipe = intent.recipe,
//           let viewController = rootViewController.viewControllers.last as? NextStepProviding {
//            viewController.nextStep(recipe: recipe)
//        }
//    }

}
