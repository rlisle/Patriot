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
            
            MenuSleepGroup()
            MenuScenesGroup()
            MenuLocationGroup()
            MenuDevicesGroup()
            Spacer()
        }
        .padding()
        .frame(maxWidth: .infinity, alignment: .leading)
        .background(Color("HeadingBackground"))
        .edgesIgnoringSafeArea(.all)
    }
}


struct MenuSleepGroup: View {

    var body: some View {
        MenuHeader(title: "Sleep State")
            .padding(.top, 100)
        
        Group {
            SleepButton(sleepState: Sleeping.Awake, icon: "sunrise", title: "Awake")
            SleepButton(sleepState: Sleeping.Retiring, icon: "bed.double", title: "Bedtime")
            SleepButton(sleepState: Sleeping.Asleep, icon: "zzz", title: "Sleeping")
        }
            .padding(.leading)
    }
}

struct MenuScenesGroup: View {
  
    @EnvironmentObject var model: PatriotModel

    var body: some View {
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
    }
}

struct MenuLocationGroup: View {

    @EnvironmentObject var model: PatriotModel

    var body: some View {
        MenuHeader(title: "Location")
            .padding(.top, 48)
        
        Group {
            Button(action: {
                print("Get Location")
                model.updateLocation()
            }) {
                MenuText(icon: "location", title: "Update Location", isSelected: false)
                .padding(.top, 30)
            }
            VStack {
                HStack {
                    Text("Lat:")
                    Spacer()
                    Text("\(model.latitude)")
                }
                HStack {
                    Text("Long:")
                    Spacer()
                    Text("\(model.longitude)")
                }
            }
            .padding(.top, 16)
            .foregroundColor(Color("AccentColor"))
            .font(.body)
        }
        .padding(.leading)
    }
}

struct MenuDevicesGroup: View {

    @EnvironmentObject var model: PatriotModel

    var body: some View {
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

struct SleepButton: View {

    @EnvironmentObject var model: PatriotModel

    var sleepState: Sleeping
    var icon: String
    var title: String
    
    var body: some View {
        
        Button(action: {
            print("Invoking sleep state \(sleepState.rawValue)")
            if let device = model.devices.first(where: {$0.name == "sleeping"}) {
                device.manualSet(percent: sleepState.rawValue)
            }

        }) {
            MenuText(icon: icon, title: title, isSelected: model.sleeping == sleepState)
            .padding(.top, 30)
        }
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
