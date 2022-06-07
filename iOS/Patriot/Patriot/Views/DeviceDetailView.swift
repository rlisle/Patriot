//
//  DeviceDetailView.swift
//  Patriot
//
//  Created by Ron Lisle on 5/22/22.
//

import SwiftUI

struct PercentSlider: View {
    @State var percent: Int = 0
    var intProxy: Binding<Double>{
        Binding<Double>(get: {
            return Double(percent)
        }, set: {
            //rounds the double to an Int
            print($0.description)
            percent = Int($0)
        })
    }
    var body: some View {
        VStack{
            Text(percent.description)
            Slider(value: intProxy , in: 0.0...100.0, step: 5.0, onEditingChanged: {_ in
                print(percent.description)
            })
        }
    }
}

struct DeviceDetailView: View {

    @EnvironmentObject var model: PatriotModel
    
    @State var isEditing: Bool = false
    
    var body: some View {
        NavigationView {
            ScrollView {
                //TODO: add more stuff'
                VStack {
                    PercentSlider(percent: model.selectedDevice.percent)
                }
            }
            .frame(maxWidth: .infinity)
            .background(Color("BackgroundColor"))
            .navigationBarTitle(model.selectedDevice.room.camelCaseToWords() + ":" + model.selectedDevice.name.camelCaseToWords())
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
