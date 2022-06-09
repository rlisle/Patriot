//
//  DeviceDetailView.swift
//  Patriot
//
//  Created by Ron Lisle on 5/22/22.
//

import SwiftUI

//struct VerticalSlider: View {
//    @EnvironmentObject var playData : PlayData
//    var channelNumber:Int
//    var sliderHeight:CGFloat
//
//    var body: some View {
//        Slider(
//            value: self.$playData.flickerDimmerValues[self.channelNumber],
//            in: 0...255,
//            step: 5.0
//        ).rotationEffect(.degrees(-90.0), anchor: .topLeading)
//        .frame(width: sliderHeight)
//        .offset(y: sliderHeight)
//    }
//}

struct VerticalPercentSlider: View {
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
            .rotationEffect(.degrees(-90.0), anchor: .topLeading)
            .frame(width: 40, height: 200)
            .offset(y: 200)
        }
    }
}

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
            .frame(width: 40, height: 200)
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
                    VerticalPercentSlider(percent: model.selectedDevice.percent)
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
