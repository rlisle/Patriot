//
//  PatriotIntentHandler.swift
//  Patriot
//
//  Created by Ron Lisle on 5/20/22.
//

import SwiftUI
import Intents

class PatriotHandler: NSObject, PatriotIntentHandling {
    
    let application: UIApplication
    
    init(application: UIApplication) {
        self.application = application
    }

//    @objc(handlePatriot:completion:)
//    func handle(intent: PatriotIntent) async -> PatriotIntentResponse

    func handle(intent: PatriotIntent, completion: @escaping (PatriotIntentResponse) -> Void) {
        
        print("PatriotHandler handle PatriotIntent")
        
            if application.applicationState == .background {
                // If app is in background, return .continueInApp to launch app
                print("AppDelegate in background")
                completion(PatriotIntentResponse(code: .continueInApp, userActivity: nil))
                
            } else {
                print("AppDelegate in foreground")
                
                // Update UI
                
                completion(PatriotIntentResponse(code: .success, userActivity: nil))
            }
        
    }
    func resolvePercent(for intent: PatriotIntent, with completion: @escaping (PatriotPercentResolutionResult) -> Swift.Void) {

        
        //TODO:
        if let percent = intent.percent as? Int {
            completion(PatriotPercentResolutionResult.success(with: percent))
        } else {
            completion(PatriotPercentResolutionResult.needsValue())
        }
        
        completion(PatriotPercentResolutionResult.success(with: intent.percent as! Int))
    }
    
}
