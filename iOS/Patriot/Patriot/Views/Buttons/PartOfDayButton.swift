//
//  PartOfDayButton.swift
//  Patriot
//
//  Created by Ron Lisle on 5/11/22.
//

import SwiftUI

struct PartOfDayButton: View {

    var body: some View {
        Button(action: {
            withAnimation {
                print("PartOfDay")
            }
        }) {
            Image(systemName: "sun.and.horizon")
                .imageScale(.large)
        }.foregroundColor(.white)
    }
}

struct PartOfDayButton_Previews: PreviewProvider {
    static var previews: some View {
        PartOfDayButton()
            .previewLayout(.sizeThatFits)
            .padding()
            .previewDisplayName("PartOfDay Button")
            .background(.black)
    }
}
