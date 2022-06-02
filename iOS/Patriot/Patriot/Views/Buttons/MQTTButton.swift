//
//  MQTTButton.swift
//  Patriot
//
//  Created by Ron Lisle on 6/2/22.
//

import SwiftUI

struct MQTTButton: View {
    
    @State var isConnected: Bool
    
    var body: some View {
        Button(action: {
            withAnimation {
                print("MQTT")
                //TODO: Cycle through states
            }
        }) {
            //TODO: Display current MQTT state
            MQTTView(isConnected: isConnected)
        }.foregroundColor(Color("TextColor"))

    }
}

struct MQTTView: View {

    @State var isConnected: Bool
    
    var body: some View {
        Image(systemName: mqttIcon(isConnected))
            .imageScale(.large)
    }
    
    func mqttIcon(_ isConnected: Bool) -> String {
        print("mqttIcon \(isConnected)")
        return isConnected ? "link.icloud" : "link.icloud.fill"
    }
}


struct MqttButton_Previews: PreviewProvider {
    static var previews: some View {
//        StatefulPreviewWrapper(false) { SideMenuButton(showMenu: $0) }
        Group {
            MQTTButton(isConnected: false)
                .previewLayout(.sizeThatFits)
                .padding()
                .previewDisplayName("Not Connected")
                .background(Color("BackgroundColor"))
            
            MQTTButton(isConnected: true)
                .previewLayout(.sizeThatFits)
                .padding()
                .previewDisplayName("Connected")
                .background(Color("BackgroundColor"))
        }
    }
}
