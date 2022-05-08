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

    var isLoggedIn: Bool {  // Convenience
        return model.photonManager.isLoggedIn
    }
    
    var body: some View {
        VStack(alignment: .leading) {
            Button(action: {
                print("Invoking Wakeup")
                if let device = model.devices.first(where: {$0.name == "sleeping"}) {
                    device.manualSet(percent: Sleeping.Awake)
                }

            }) {
                HStack {
                    Image(systemName: "sunrise")
                        .foregroundColor(.gray)
                        .imageScale(.large)
                    Text("Awake")
                        .foregroundColor(.gray)
                        .font(.headline)
                }
                .padding(.top, 100)
            }
            Button(action: {
                print("Invoking Bedtime")
                if let device = model.devices.first(where: {$0.name == "sleeping"}) {
                    device.manualSet(percent: Sleeping.Retiring)
                }

            }) {
                HStack {
                    Image(systemName: "bed.double")
                        .foregroundColor(.gray)
                        .imageScale(.large)
                    Text("Bedtime")
                        .foregroundColor(.gray)
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
                        .foregroundColor(.gray)
                        .imageScale(.large)
                    Text("Watch TV")
                        .foregroundColor(.gray)
                        .font(.headline)
                }
                .padding(.top, 30)
            }
            Button(action: {
                print(isLoggedIn ? "Logout" : "Login")
                if isLoggedIn {
                    model.logout()
                } else {
                    model.showingLogin = true
                }
            }) {
                HStack {
                    Image(systemName: "person")
                        .foregroundColor(.gray)
                        .imageScale(.large)
                    Text("Logout")
                        .foregroundColor(.gray)
                        .font(.headline)
                }
                .padding(.top, 30)
            }
            Spacer()
        }
        .padding()
        .frame(maxWidth: .infinity, alignment: .leading)
        .background(Color(red: 32/255, green: 32/255, blue: 32/255))
        .edgesIgnoringSafeArea(.all)
    }
}

struct MenuView_Previews: PreviewProvider {
    static var previews: some View {
        MenuView()
    }
}
