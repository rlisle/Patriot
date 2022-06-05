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
            
            Text("Sleep State")
                .foregroundColor(Color("HeadingTextColor"))
                .font(.headline)
                .padding(.top, 100)
            
            Group {
                Button(action: {
                    print("Invoking Wakeup")
                    if let device = model.devices.first(where: {$0.name == "sleeping"}) {
                        device.manualSet(percent: Sleeping.Awake.rawValue)
                    }

                }) {
                    HStack {
                        Image(systemName: "sunrise")
                            .foregroundColor(Color("HeadingTextColor"))
                            .imageScale(.large)
                        Text("Awake")
                            .foregroundColor(Color("HeadingTextColor"))
                            .font(.headline)
                    }
                    .padding(.top, 30)
                }
                
                Button(action: {
                    print("Invoking Bedtime")
                    if let device = model.devices.first(where: {$0.name == "sleeping"}) {
                        device.manualSet(percent: Sleeping.Retiring.rawValue)
                    }

                }) {
                    HStack {
                        Image(systemName: "bed.double")
                            .foregroundColor(Color("HeadingTextColor"))
                            .imageScale(.large)
                        Text("Bedtime")
                            .foregroundColor(Color("HeadingTextColor"))
                            .font(.headline)
                    }
                    .padding(.top, 30)
                }
                
                Button(action: {
                    print("Invoking Watch TV")
                    if let device = model.devices.first(where: {$0.name == "watching"}) {
                        device.manualSet(percent: 100)
                    }
                }) {
                    HStack {
                        Image(systemName: "tv")
                            .foregroundColor(Color("HeadingTextColor"))
                            .imageScale(.large)
                        Text("Watch TV")
                            .foregroundColor(Color("HeadingTextColor"))
                            .font(.headline)
                    }
                    .padding(.top, 30)
                }
            }
            .padding(.leading)

            Text("Device Control")
                .foregroundColor(Color("HeadingTextColor"))
                .font(.headline)
                .padding(.top, 48)
            

            Group {
                
                Button(action: {
                    print("Reset Devices")
                    model.resetDevices()
                }) {
                    HStack {
                        Image(systemName: "tv")
                            .foregroundColor(Color("HeadingTextColor"))
                            .imageScale(.large)
                        Text("Reset Devices")
                            .foregroundColor(Color("HeadingTextColor"))
                            .font(.headline)
                    }
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

struct MenuView_Previews: PreviewProvider {
    static var previews: some View {
        MenuView()
            .environmentObject(PatriotModel(testMode: .on))
            .previewLayout(PreviewLayout.fixed(width: 200, height: 800))
            .previewDisplayName("Side Menu")
    }
}
