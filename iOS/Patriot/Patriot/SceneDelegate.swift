//
//  SceneDelegate.swift
//  Patriot
//
//  Created by Ron Lisle on 5/16/22.
//

import SwiftUI

class SceneDelegate: NSObject, UIWindowSceneDelegate, ObservableObject {
    
    func sceneWillEnterForeground(_ scene: UIScene) {
        print("sceneWillEnterForeground")
    }
    
    func sceneDidBecomeActive(_ scene: UIScene) {
        print("sceneDidBecomeActive")
    }
    
    func sceneWillResignActive(_ scene: UIScene) {
        print("sceneWillResignActive")
    }
}
