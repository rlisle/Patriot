//
//  MainView.swift
//  Patriot
//
//  Created by Ron Lisle on 7/13/21.
//

import SwiftUI

struct MainView: View {
    
    @Binding var showMenu: Bool
    @EnvironmentObject var model: PatriotModel

    let columns = [
        GridItem(.adaptive(minimum: 80, maximum: 160))
    ]

    var body: some View {
        VStack {
            ScrollView {
// TODO: refactor to SectionView (currently data is lost)
//                SectionView(title: "Favorites", devices:
//                    model.devices.filter { $0.isFavorite }
//                )
//                SectionView(title: "Living Room", devices:
//                    model.devices.filter { $0.isFavorite == false }
//                )

                // Favorites
                if model.devices.filter { $0.isFavorite == true }.count > 0 {
                    FavoritesView().environmentObject(model)
                }
                
                // Rooms
                ForEach(model.rooms, id: \.self) { room in
                
//                    SectionView(title: room, devices: model.devices.filter { /*$0.isFavorite == false  &&*/ $0.room == room})
//                        .environmentObject(model)
                    RoomView(room: room)
                }

                
            }
        }
        .padding(.top, 16)
        .background(Color(.black).ignoresSafeArea())
        .sheet(isPresented: $model.showingLogin) {
            LoginView()
        }
    }
}

struct FavoritesView: View {
    
    @EnvironmentObject var model: PatriotModel
    
    let columns = [
        GridItem(.adaptive(minimum: 80, maximum: 160))
    ]
    
    var body: some View {
        Section(
            header:
                HStack {
                    Spacer()
                    Text("Favorites")
                    Spacer()
                }
                .foregroundColor(.white)
                .background(Color(red: 0.15, green: 0.15, blue: 0.15))
            ) {
                LazyVGrid(columns: columns, spacing: 20) {
                    ForEach(model.devices.filter { $0.isFavorite }, id: \.self) { device in
                        DeviceView(device: device)
                            .aspectRatio(1, contentMode: .fill)
                            .padding()
                    }
                }
            }
        .padding(.horizontal)
    }
}

struct RoomView: View {
    
    @EnvironmentObject var model: PatriotModel
    var room: String
    
    let columns = [
        GridItem(.adaptive(minimum: 80, maximum: 160))
    ]
    
    var body: some View {
        Section(
            header:
                HStack {
                    Spacer()
                    Text(room)
                    Spacer()
                }
                .foregroundColor(.white)
                .background(Color(red: 0.15, green: 0.15, blue: 0.15))
            ) {
                LazyVGrid(columns: columns, spacing: 20) {
                    ForEach(model.devices.filter { /*$0.isFavorite == false  &&*/ $0.room == room}, id: \.self) { device in
                        DeviceView(device: device)
                            .aspectRatio(1, contentMode: .fill)
                            .padding()
                    }
                }
            }
        .padding(.horizontal)
    }
}

struct MainView_Previews: PreviewProvider {
    static var previews: some View {
        StatefulPreviewWrapper(true) { MainView(showMenu: $0) }
            .environmentObject(PatriotModel(forTest: true))
    }
}
