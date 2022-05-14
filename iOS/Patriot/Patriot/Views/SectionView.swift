//
//  SectionView.swift
//  Patriot
//
//  Created by Ron Lisle on 7/18/21.
//

import SwiftUI

struct SectionView: View {
    
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
                .background(Color(.gray))
            ) {
                LazyVGrid(columns: columns, spacing: 20) {
                    ForEach(model.devices.filter { $0.room == room }, id: \.self) { device in
                        DeviceView(device: device)
                            .aspectRatio(1, contentMode: .fill)
                            .padding()
                    }
                }
            }
            .background(Color(.black).ignoresSafeArea())
        .padding(.horizontal)
    }
}

struct SectionView_Previews: PreviewProvider {
    static var previews: some View {
        SectionView(room: "Living Room")
            .environmentObject(PatriotModel(forTest: true))
    }
}
