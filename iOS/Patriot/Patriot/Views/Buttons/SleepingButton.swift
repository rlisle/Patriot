//
//  SleepingButton.swift
//  Patriot
//
//  Created by Ron Lisle on 5/11/22.
//

import SwiftUI

struct SleepingButton: View {
    var body: some View {
        Button(action: {
            withAnimation {
                print("Sleeping")
                //TODO: Cycle through states
            }
        }) {
            //TODO: Display current sleeping state
            SleepView(sleeping: .Awake)
        }.foregroundColor(.white)

    }
}

struct SleepView: View {

    @State var sleeping: Sleeping
    
    var body: some View {
        Image(systemName: sleepIcon())
            .imageScale(.large)
    }
    
    func sleepIcon() -> String {
        return "bed.double.circle"
    }
}


struct SleepingButton_Previews: PreviewProvider {
    static var previews: some View {
//        StatefulPreviewWrapper(false) { SideMenuButton(showMenu: $0) }
//            .environmentObject(PatriotModel(forTest: true))
        SleepingButton()
            .previewLayout(.sizeThatFits)
            .padding()
            .previewDisplayName("Sleeping Button")
            .background(.black)
    }
}
