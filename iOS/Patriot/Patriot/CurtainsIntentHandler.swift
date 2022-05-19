//
//  CurtainsIntentHandler.swift
//  Patriot
//
//  Created by Ron Lisle on 5/18/22.
//

import SwiftUI
import Intents

class CurtainsHandler: NSObject, CurtainsIntentHandling {
    
    let application: UIApplication
    
    init(application: UIApplication) {
        self.application = application
    }

//    @objc(handleCurtains:completion:)
//    func handle(intent: CurtainsIntent) async -> CurtainsIntentResponse

    func handle(intent: CurtainsIntent, completion: @escaping (CurtainsIntentResponse) -> Void) {
        
        print("AppDelegate handle Curtains")
        
            if application.applicationState == .background {
                // If app is in background, return .continueInApp to launch app
                print("AppDelegate in background")
                completion(CurtainsIntentResponse(code: .continueInApp, userActivity: nil))
                
            } else {
                print("AppDelegate in foreground")
                
                // Update UI
                
                completion(CurtainsIntentResponse(code: .success, userActivity: nil))
            }
        
    }
    func resolvePercent(for intent: CurtainsIntent, with completion: @escaping (CurtainsPercentResolutionResult) -> Swift.Void) {

        
        //TODO:
        if let percent = intent.percent as? Int {
            completion(CurtainsPercentResolutionResult.success(with: percent))
        } else {
            completion(CurtainsPercentResolutionResult.needsValue())
        }
        
        completion(CurtainsPercentResolutionResult.success(with: intent.percent as! Int))
    }
    
}
