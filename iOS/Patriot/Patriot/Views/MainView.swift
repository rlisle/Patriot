//
//  MainView.swift
//  Patriot
//
//  Created by Ron Lisle on 7/13/21.
//

import SwiftUI

struct MainView: View {
    
    @EnvironmentObject var model: PatriotModel

    var body: some View {
        ScrollView {

            // Favorites
            if model.devices.filter { $0.isFavorite == true }.count > 0 {
                SectionView(section: "Favorites")
            }

            // Rooms
            ForEach(model.rooms, id: \.self) { room in
                SectionView(section: room)
                
            }
        }
        .background(Color("BackgroundColor"))
    }
}

struct MainView_Previews: PreviewProvider {
    static var previews: some View {
        MainView()
            .environmentObject(PatriotModel(testMode: .on))
    }
}
