//
//  SleepingButton.swift
//  Patriot
//
//  Created by Ron Lisle on 5/11/22.
//

import SwiftUI

struct SleepingButton: View {
    
    @State var sleeping: Sleeping
    
    var body: some View {
        Button(action: {
            withAnimation {
                print("Sleeping")
                //TODO: Cycle through states
            }
        }) {
            //TODO: Display current sleeping state
            SleepView(sleeping: sleeping)
        }.foregroundColor(.white)

    }
}

struct SleepView: View {

    @State var sleeping: Sleeping
    
    var body: some View {
        Image(systemName: sleepIcon(sleeping))
            .imageScale(.large)
    }
    
    func sleepIcon(_ sleeping: Sleeping) -> String {
        print("sleepIcon \(sleeping)")
        switch sleeping {
        case .Awake:
            return "figure.walk"
        case .Retiring:
            return "bed.double"
        case .Asleep:
            return "moon.zzz"
        default:
            return "questionmark"
        }
    }
}


struct SleepingButton_Previews: PreviewProvider {
    static var previews: some View {
//        StatefulPreviewWrapper(false) { SideMenuButton(showMenu: $0) }
        Group {
            SleepingButton(sleeping: .Awake)
                .previewLayout(.sizeThatFits)
                .padding()
                .previewDisplayName("Awake")
                .background(.black)
            
            SleepingButton(sleeping: .Retiring)
                .previewLayout(.sizeThatFits)
                .padding()
                .previewDisplayName("Bedtime")
                .background(.black)

            SleepingButton(sleeping: .Asleep)
                .previewLayout(.sizeThatFits)
                .padding()
                .previewDisplayName("Sleeping")
                .background(.black)
        }
    }
}
