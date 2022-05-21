//
//  RVIntentHandler.swift
//  Patriot
//
//  Created by Ron Lisle on 5/20/22.
//

import SwiftUI
import Intents

class RVHandler: NSObject, RVIntentHandling {
    
    let application: UIApplication
    
    init(application: UIApplication) {
        self.application = application
    }

    //TODO: convert to async/await (see protocol)

    func handle(intent: RVIntent, completion: @escaping (RVIntentResponse) -> Void) {
        
        print("RVHandler handle RVIntent")
        
        guard let device = intent.device,
              let percent = intent.percent as? Int else {
            print("handle(intent) missing arguments")
            completion(RVIntentResponse(code: .failure, userActivity: nil))
            return
        }
        
        if application.applicationState == .background {
            // If app is in background, return .continueInApp to launch app
            print("RVHandler in background")

            PatriotModel.shared.sendMessage(topic: "patriot/\(device)", message: "\(percent)")
            
            completion(RVIntentResponse(code: .success, userActivity: nil))
            //completion(RVIntentResponse(code: .continueInApp, userActivity: nil))
            
        } else {
            print("RVHandler in foreground")
            
            // Update UI
            
            completion(RVIntentResponse(code: .success, userActivity: nil))
        }
    }
    
    func resolveDevice(for intent: RVIntent, with completion: @escaping (INStringResolutionResult) -> Swift.Void) {
        
        guard let device = intent.device else {
            print("RVHandler resolveDevice: needsValue")
            completion(INStringResolutionResult.needsValue())
            return
        }
        
        print("RVHandler resolveDevice: \(device)")
        
        guard PatriotModel.shared.getDevice(name: device) != nil else {
            print("RVHandler resolveDevice: unrecognized device \(device)")
            completion(INStringResolutionResult.needsValue())
            return
        }
        
        print("RVHandler resolveDevice: recognized device \(device)")
        completion(INStringResolutionResult.success(with: device))
    }
    

    func resolvePercent(for intent: RVIntent, with completion: @escaping (RVPercentResolutionResult) -> Swift.Void) {
        
        print("RVHandler resolvePercent")

        //TODO:
        guard let percent = intent.percent as? Int else {
            print("RVHandler resolvePercent: needsValue")
            completion(RVPercentResolutionResult.needsValue())
            return
        }
        var goodPercent = percent
        if goodPercent > 100 { goodPercent = 100 }
        if goodPercent < 0 { goodPercent = 0 }
        completion(RVPercentResolutionResult.success(with: goodPercent))
    }
    
    //TODO: Add confirm method
    
}
