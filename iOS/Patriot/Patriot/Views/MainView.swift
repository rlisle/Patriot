//
//  MainView.swift
//  Patriot
//
//  Created by Ron Lisle on 7/13/21.
//

import SwiftUI

struct MainView: View {
    
    @EnvironmentObject var model: PatriotModel

    @Binding var showMenu: Bool
    
    let columns = [
        GridItem(.adaptive(minimum: 80, maximum: 160))
    ]

    var body: some View {
        VStack {
            ScrollView {

                SectionView(title: "Favorites", devices:
                    model.devices.filter { $0.isFavorite }
                )
                SectionView(title: "Living Room", devices:
                    model.devices.filter { $0.isFavorite == false }
                )
            }
        }
        .padding(.top, 16)
        .background(Color(.black).ignoresSafeArea())
        .sheet(isPresented: $model.needsLogIn) {
            LoginView()
        }
    }
}

struct MainView_Previews: PreviewProvider {
    static var previews: some View {
        StatefulPreviewWrapper(true) { MainView(showMenu: $0) }
            .environmentObject(PatriotModel(forTest: true))
    }
}
