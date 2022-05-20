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

    //TODO: convert to async/await (see protocol)

    func handle(intent: PatriotIntent, completion: @escaping (PatriotIntentResponse) -> Void) {
        
        print("PatriotHandler handle PatriotIntent")
        
        guard let device = intent.device,
              let percent = intent.percent as? Int else {
            print("handle(intent) missing arguments")
            completion(PatriotIntentResponse(code: .failure, userActivity: nil))
            return
        }
        
        if application.applicationState == .background {
            // If app is in background, return .continueInApp to launch app
            print("PatriotHandler in background")

            PatriotModel.shared.sendMessage(topic: "patriot/\(device)", message: "\(percent)")
            
            completion(PatriotIntentResponse(code: .success, userActivity: nil))
            //completion(PatriotIntentResponse(code: .continueInApp, userActivity: nil))
            
        } else {
            print("PatriotHandler in foreground")
            
            // Update UI
            
            completion(PatriotIntentResponse(code: .success, userActivity: nil))
        }
    }
    
    func resolveDevice(for intent: PatriotIntent, with completion: @escaping (INStringResolutionResult) -> Swift.Void) {

        print("PatriotHandler resolveDevice")
        
        //TODO:
        if let device = intent.device as? String {
            completion(INStringResolutionResult.success(with: device))
        } else {
            completion(INStringResolutionResult.needsValue())
        }
    }
    

    func resolvePercent(for intent: PatriotIntent, with completion: @escaping (PatriotPercentResolutionResult) -> Swift.Void) {
        
        print("PatriotHandler resolvePercent")

        //TODO:
        if let percent = intent.percent as? Int {
            completion(PatriotPercentResolutionResult.success(with: percent))
        } else {
            completion(PatriotPercentResolutionResult.needsValue())
        }
    }
    
    //TODO: Add confirm method
    
}
