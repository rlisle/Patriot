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
        GridItem(.adaptive(minimum: 100, maximum: 160))
    ]
    
    var body: some View {
        Section(
            header:
                HStack {
                    Spacer()
                    Text(section)
                    Spacer()
                }
                .padding(.vertical, 8)
                .foregroundColor(Color("HeadingTextColor"))
                .background(Color("HeadingBackground"))

            ) {
                LazyVGrid(columns: columns, spacing: 8)
                {
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
    }
}

struct SectionView_Previews: PreviewProvider {
    static var previews: some View {
        SectionView(section: "Favorites")
            .environmentObject(PatriotModel(forTest: true))
    }
}
