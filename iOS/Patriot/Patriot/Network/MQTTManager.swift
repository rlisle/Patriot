//
//  mqttManager.swift
//  Patriot
//
//  Created by Ron Lisle on 6/10/18.
//  Copyright © 2018 Rons iMac. All rights reserved.
//

import Foundation
import CocoaMQTT

// This protocol is used to send MQTT events
protocol MQTTSending
{
    func sendMessage(topic: String, message: String)
    func sendPatriotMessage(device: String, percent: Int)
}

// This protocol provides notifications of MQTT events to a delegate
protocol MQTTReceiving
{
    func connectionDidChange(isConnected: Bool)
    func didReceiveMessage(topic: String, message: String)
}

class MQTTManager {

    let mqttURL = "192.168.50.33"
    let mqttPort: UInt16 = 1883
    let mqttTopic = "#"             // For now we're receiving everything
    
    let mqtt: CocoaMQTT!
    
    var isConnected: Bool = false
    var testMode: TestMode = .off
    
    var mqttDelegate: MQTTReceiving?
    
    init(testMode: TestMode = .off) {
        self.testMode = testMode
        let clientID = "Patriot" + String(ProcessInfo().processIdentifier)
        mqtt = CocoaMQTT(clientID: clientID, host: mqttURL, port: mqttPort) // TODO: mqtt5?
        mqtt.delegate = self
        reconnect()
    }
    
    func reconnect() {
        guard testMode == .off else {
            print("MQTT testing so not connecting")
            return
        }
        isConnected = mqtt.connect()
    }
}

extension MQTTManager: MQTTSending
{
    func sendMessage(topic: String, message: String) {
        guard testMode == .off else {
            print("sendMessage - testMode set so not sending \(topic), \(message)")
            return
        }
        guard isConnected == true else {
            print("No MQTT: Cannot send \(topic), \(message)")
            return
        }
        print("sendMessage - sending \(topic), \(message)")
        mqtt.publish(topic, withString: message)
    }
    
    func sendPatriotMessage(device: String, percent: Int)
    {
        guard testMode == .off else {
            return
        }
        guard isConnected == true else {
            print("No MQTT: Cannot send patriot/\(device), \(percent)")
            return
        }
        let topic = "patriot/"+device
        let message = String(percent)
        sendMessage(topic: topic, message: message)
    }
}

extension MQTTManager: CocoaMQTTDelegate {
        
    func mqtt(_ mqtt: CocoaMQTT, didConnectAck ack: CocoaMQTTConnAck) {
        print("MQTT didConnectAck")
        mqtt.subscribe(mqttTopic)
        mqttDelegate?.connectionDidChange(isConnected: true)
    }

    func mqtt(_ mqtt: CocoaMQTT, didPublishMessage message: CocoaMQTTMessage, id: UInt16) {
        //print("MQTT didPublishMessage: \(message.topic), \(String(describing: message.string))")
    }
    
    func mqtt(_ mqtt: CocoaMQTT, didPublishAck id: UInt16) {
        //print("MQTT didPublishAck")
    }
    
    func mqtt(_ mqtt: CocoaMQTT, didReceiveMessage message: CocoaMQTTMessage, id: UInt16 ) {
        if let payload: String = message.string
        {
            let topic = message.topic
            //print("MQTT didReceiveMessage: \(topic), \(payload)")
            self.mqttDelegate?.didReceiveMessage(topic: topic, message: payload)
        }
    }

    func mqtt(_ mqtt: CocoaMQTT, didSubscribeTopics success: NSDictionary, failed: [String]) {
        for topic in success {
            print("MQTT didSubscribeTopic: \(topic)")
        }
    }
    
    func mqtt(_ mqtt: CocoaMQTT, didUnsubscribeTopics topics: [String]) {
        print("MQTT didUnsubscribeTopic: \(topics)")
    }

    func mqttDidPing(_ mqtt: CocoaMQTT) {
        //print("MQTT ping")
    }
    
    func mqttDidReceivePong(_ mqtt: CocoaMQTT) {
        //print("MQTT pong")
    }
    
    func mqttDidDisconnect(_ mqtt: CocoaMQTT, withError err: Error?) {
        print("MQTT didDisconnect withError: \(String(describing: err))")
        isConnected = false
        mqttDelegate?.connectionDidChange(isConnected: false)
    }

    func mqtt(_ mqtt: CocoaMQTT, didReceive trust: SecTrust, completionHandler: @escaping (Bool) -> Void) {
        print("MQTT didReceive trust: \(trust)")
        completionHandler(true)
    }
    
    func mqtt(_ mqtt: CocoaMQTT, didPublishComplete id: UInt16) {
        //print("MQTT didPublishAck id: \(id)")
    }
    
    func mqtt(_ mqtt: CocoaMQTT, didStateChangeTo state: CocoaMQTTConnState) {
        print("MQTT didStateChange \(state), doing nothing.")
    }

    func _console(_ info: String) {
        //AdLog.text("MQTT console: \(info)", type: .debug)
    }
}
