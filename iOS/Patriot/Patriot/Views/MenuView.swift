//
//  MenuView.swift
//  Patriot
//
//  Side Menu
//
//  Created by Ron Lisle on 7/13/21.
//

import SwiftUI

struct MenuView: View {
    
    @EnvironmentObject var model: PatriotModel

    var body: some View {

        VStack(alignment: .leading, spacing: 0){
            
            MenuHeader(title: "Sleep State")
                .padding(.top, 100)
            
            Group {
                Button(action: {
                    print("Invoking Wakeup")
                    if let device = model.devices.first(where: {$0.name == "sleeping"}) {
                        device.manualSet(percent: Sleeping.Awake.rawValue)
                    }

                }) {
                    MenuText(icon: "sunrise", title: "Awake", isSelected: model.sleeping == .Awake)
                    .padding(.top, 30)
                }
                
                Button(action: {
                    print("Invoking Bedtime")
                    if let device = model.devices.first(where: {$0.name == "sleeping"}) {
                        device.manualSet(percent: Sleeping.Retiring.rawValue)
                    }

                }) {
                    MenuText(icon: "bed.double", title: "Bedtime", isSelected: model.sleeping == .Retiring)
                    .padding(.top, 30)
                }
                
                Button(action: {
                    print("Invoking Asleep")
                    if let device = model.devices.first(where: {$0.name == "sleeping"}) {
                        device.manualSet(percent: Sleeping.Asleep.rawValue)
                    }

                }) {
                    MenuText(icon: "zzz", title: "Sleeping", isSelected: model.sleeping == .Asleep)
                    .padding(.top, 30)
                }
            }
            .padding(.leading)

            MenuHeader(title: "Scenes")
                .padding(.top, 48)

            Group {
                
                Button(action: {
                    print("Invoking Watch TV")
                    if let device = model.devices.first(where: {$0.name == "watching"}) {
                        device.manualSet(percent: 100)
                    }
                }) {
                    MenuText(icon: "tv", title: "Watch TV", isSelected: false)
                    .padding(.top, 30)
                }
            
            }
            .padding(.leading)

            MenuHeader(title: "Device Control")
                .padding(.top, 48)
            

            Group {
                
                Button(action: {
                    print("Reset Devices")
                    model.resetDevices()
                }) {
                    MenuText(icon: "repeat", title: "Reset Devices", isSelected: false)
                    .padding(.top, 30)
                }
            }
            .padding(.leading)
            
            Spacer()
        }
        .padding()
        .frame(maxWidth: .infinity, alignment: .leading)
        .background(Color("HeadingBackground"))
        .edgesIgnoringSafeArea(.all)
    }
}


struct MenuHeader: View {
    
    var title: String
    
    var body: some View {
        Text(title)
            .foregroundColor(Color("HeadingTextColor"))
            .font(.headline)
    }
}

struct MenuText: View {
    
    var icon: String
    var title: String
    var isSelected: Bool
    
    var body: some View {
        HStack {
            Image(systemName: icon)
                .imageScale(.large)
            Text(title)
                .font(.headline)
            if isSelected {
                Image(systemName: "checkmark")
                    .imageScale(.large)
            }
        }
        .foregroundColor(Color(isSelected ? "AccentColor" : "HeadingTextColor"))
    }
}


struct MenuView_Previews: PreviewProvider {
    static var previews: some View {
        MenuView()
            .environmentObject(PatriotModel(testMode: .on))
            .previewLayout(PreviewLayout.fixed(width: 200, height: 800))
            .previewDisplayName("Side Menu")
    }
}
