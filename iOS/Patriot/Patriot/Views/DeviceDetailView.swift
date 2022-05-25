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
        //TODO: add more stuff'
        VStack {
            Text("Room: \(model.selectedDevice.room)")
            Text(model.selectedDevice.name)
        }
    }
}

struct DeviceDetailView_Previews: PreviewProvider {
    static var previews: some View {
        DeviceDetailView()
            .environmentObject(PatriotModel(forTest: true))
    }
}
