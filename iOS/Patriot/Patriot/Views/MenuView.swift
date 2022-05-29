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
        VStack(alignment: .leading) {
            Button(action: {
                print("Invoking Wakeup")
                if let device = model.devices.first(where: {$0.name == "sleeping"}) {
                    device.manualSet(percent: Sleeping.Awake.rawValue)
                }

            }) {
                HStack {
                    Image(systemName: "sunrise")
                        .foregroundColor(Color("TextColor"))
                        .imageScale(.large)
                    Text("Awake")
                        .foregroundColor(Color("TextColor"))
                        .font(.headline)
                }
                .padding(.top, 100)
            }
            Button(action: {
                print("Invoking Bedtime")
                if let device = model.devices.first(where: {$0.name == "sleeping"}) {
                    device.manualSet(percent: Sleeping.Retiring.rawValue)
                }

            }) {
                HStack {
                    Image(systemName: "bed.double")
                        .foregroundColor(Color("TextColor"))
                        .imageScale(.large)
                    Text("Bedtime")
                        .foregroundColor(Color("TextColor"))
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
                        .foregroundColor(Color("TextColor"))
                        .imageScale(.large)
                    Text("Watch TV")
                        .foregroundColor(Color("TextColor"))
                        .font(.headline)
                }
                .padding(.top, 30)
            }
            Button(action: {
                print("Reset Devices")
                model.resetDevices()
            }) {
                HStack {
                    Image(systemName: "tv")
                        .foregroundColor(Color("TextColor"))
                        .imageScale(.large)
                    Text("Reset Devices")
                        .foregroundColor(Color("TextColor"))
                        .font(.headline)
                }
                .padding(.top, 30)
            }
            Spacer()
        }
        .padding()
        .frame(maxWidth: .infinity, alignment: .leading)
        .background(Color("BackgroundColor"))
        .edgesIgnoringSafeArea(.all)
    }
}

struct MenuView_Previews: PreviewProvider {
    static var previews: some View {
        MenuView()
    }
}
