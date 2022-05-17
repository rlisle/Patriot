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
    
    func scene(_ scene: UIScene, continue userActivity: NSUserActivity) {
        handleUserActivity(userActivity)
    }
    
    func handleUserActivity(_ userActivity: NSUserActivity) {
        
        print("handleUserActivity")
        
        guard let window = window,
            let rootViewController = window.rootViewController as? UINavigationController,
            let interaction = userActivity.interaction else {
                print("handlerUserActivity missing info")
                return
        }

//        if let intent = interaction.intent as? ShowDirectionsIntent,
//           let recipe = intent.recipe,
//           let viewController = rootViewController.viewControllers.last as? NextStepProviding {
//            viewController.nextStep(recipe: recipe)
//        }
    }

}
