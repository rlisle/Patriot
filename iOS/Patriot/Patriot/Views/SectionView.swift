//
//  SectionView.swift
//  Patriot
//
//  Created by Ron Lisle on 7/18/21.
//

import SwiftUI

struct SectionView: View {
    
    @EnvironmentObject var model: PatriotModel
    var section: String

    let columns = [
        GridItem(.adaptive(minimum: 80, maximum: 160))
    ]
    
    var body: some View {
        Section(
            header:
                HStack {
                    Spacer()
                    Text(section)
                    Spacer()
                }
                .foregroundColor(Color("HeadingTextColor"))
                .padding(.vertical, 8)
                .padding(.horizontal, 0)
                .background(Color("HeadingBackground"))
            ) {
                LazyVGrid(columns: columns, spacing: 8) {
                    if section == "Favorites" {
                        ForEach(model.devices.filter { $0.isFavorite }, id: \.self) { device in
                            DeviceView(device: device)
                            .aspectRatio(1, contentMode: .fill)
                            .padding(4)
                        }

                    } else {
                        ForEach(model.devices.filter { $0.room == section }, id: \.self) { device in
                            DeviceView(device: device)
                                .aspectRatio(1, contentMode: .fill)
                                .padding(4)
                        }
                    }
                }
            }
            .background(Color("BackgroundColor").ignoresSafeArea())
        .padding(.horizontal)
    }
}

struct SectionView_Previews: PreviewProvider {
    static var previews: some View {
//        SectionView(section: "LivingRoom")
//            .environmentObject(PatriotModel(forTest: true))
        SectionView(section: "Favorites")
            .environmentObject(PatriotModel(forTest: true))
    }
}
