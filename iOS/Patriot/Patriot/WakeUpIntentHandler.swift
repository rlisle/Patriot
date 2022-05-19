//
//  WakeUpIntentHandler.swift
//  Patriot
//
//  Created by Ron Lisle on 5/18/22.
//

import SwiftUI

class WakingUpHandler: NSObject, WakingUpIntentHandling {
    let application: UIApplication
    
    init(application: UIApplication) {
        self.application = application
    }
    
    func handle(intent: WakingUpIntent,
        completion: @escaping (WakingUpIntentResponse) -> Void) {
        
        print("WakingUpHandler handle WakingUp")
        
        if application.applicationState == .background {
            print("WakingUpHandler in background")
//            PatriotModel.shared.mqtt.sendMessage(topic: "patriot/sleeping", message: "\(Sleeping.Awake)")
            completion(WakingUpIntentResponse(code: .continueInApp, userActivity: nil))
            
        } else {
            print("WakingUpHandler in foreground")
            PatriotModel.shared.mqtt.sendMessage(topic: "patriot/sleeping", message: "\(Sleeping.Awake)")
            completion(WakingUpIntentResponse(code: .continueInApp, userActivity: nil))

            // Update UI
            
            completion(WakingUpIntentResponse(code: .success, userActivity: nil))
        }
        
    }
}
