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

//class AppDelegate: NSObject, UIApplicationDelegate {
//    func application(_ application: UIApplication, handlerFor intent: INIntent) -> Any? {
//        switch intent {
//        case is GoodnightIntent:
//            return GoodnightHandler()
//        default:
//            print("Unhandled intent")
//            return nil
//        }
//    }
//}
//
//class GoodnightHandler: NSObject, ProcessGoodnightIntentHandling {
//    let application: UIApplication
//    
//    init(application: UIApplication) {
//        self.application = application
//    }
//    
//    func handle(intent: ProcessGoodnightIntent,
//        completion: @escaping (ProcessGoodnightResponse) -> Void {
//            if application.state == .background {
//                completion(ProcessGoodnightResponse(code: .continueInApp, userActivity: nil))
//            } else {
//                
//                // Update UI
//                
//                completion(ProcessGoodnightResponse(code: .success, userActivity: nil))
//            }
//        }
//    )
//}
