//
//  SleepingButton.swift
//  Patriot
//
//  Created by Ron Lisle on 5/11/22.
//

import SwiftUI

struct SleepingButton: View {
    
    @EnvironmentObject var model: PatriotModel
    
    var body: some View {
        Button(action: {
            withAnimation {
                print("Sleeping")
                //TODO: Cycle through states
            }
        }) {
            //TODO: Display current sleeping state
            SleepView()
        }.foregroundColor(.white)

    }
}

struct SleepView: View {

    @EnvironmentObject var model: PatriotModel
    
    var body: some View {
        Image(systemName: sleepIcon())
            .imageScale(.large)
    }
    
    func sleepIcon() -> String {
        switch model.sleeping {
        case .Awake:
            return "figure.walk"
        case .Retiring:
            return "bed.double"
        case .Asleep:
            return "moon.zzz"
        }
    }
}


struct SleepingButton_Previews: PreviewProvider {
    static var previews: some View {
//        StatefulPreviewWrapper(false) { SideMenuButton(showMenu: $0) }
        Group {
            SleepingButton()
                .environmentObject(PatriotModel(forTest: true, sleeping: .Awake))
                .previewLayout(.sizeThatFits)
                .padding()
                .previewDisplayName("Awake")
                .background(.black)
            
            SleepingButton()
                .environmentObject(PatriotModel(forTest: true, sleeping: .Retiring))
                .previewLayout(.sizeThatFits)
                .padding()
                .previewDisplayName("Bedtime")
                .background(.black)

            SleepingButton()
                .environmentObject(PatriotModel(forTest: true, sleeping: .Asleep))
                .previewLayout(.sizeThatFits)
                .padding()
                .previewDisplayName("Sleeping")
                .background(.black)
        }
    }
}
