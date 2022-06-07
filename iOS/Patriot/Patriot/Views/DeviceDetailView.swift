//
//  DeviceDetailView.swift
//  Patriot
//
//  Created by Ron Lisle on 5/22/22.
//

import SwiftUI

struct DeviceDetailView: View {

    @EnvironmentObject var model: PatriotModel
    
    var body: some View {
        NavigationView {
            ScrollView {
                //TODO: add more stuff'
                VStack {
                    Text("Room: \(model.selectedDevice.room)")
                    Text(model.selectedDevice.name)
                }
            }
            .frame(maxWidth: .infinity)
            .background(Color("BackgroundColor"))
            .navigationBarTitle(model.selectedDevice.name.camelCaseToWords())
            .navigationBarTitleDisplayMode(.inline)
            .foregroundColor(Color("TextColor"))

        }
    }
}

struct DeviceDetailView_Previews: PreviewProvider {
    static var previews: some View {
        DeviceDetailView()
            .environmentObject(PatriotModel(testMode: .on))
    }
}
