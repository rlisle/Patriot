//
//  SideMenuButton.swift
//  Patriot
//
//  Created by Ron Lisle on 5/11/22.
//

import SwiftUI

struct SideMenuButton: View {

    @Binding var showMenu: Bool

    var body: some View {

        Button(action: {
            withAnimation {
                self.showMenu.toggle()
            }
        }) {
            Image(systemName: "line.horizontal.3")
                .imageScale(.large)
        }.foregroundColor(.white)
    }
}

struct SideMenuButton_Previews: PreviewProvider {
    static var previews: some View {
        StatefulPreviewWrapper(false) { SideMenuButton(showMenu: $0) }
            .environmentObject(PatriotModel(forTest: true))
            .previewLayout(.sizeThatFits)
            .padding()
            .previewDisplayName("SideMenu Button")
            .background(.black)
    }
}
