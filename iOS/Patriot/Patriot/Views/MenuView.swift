//
//  MenuView.swift
//  Patriot
//
//  Side Menu
//
//  Created by Ron Lisle on 7/13/21.
//

import SwiftUI

struct MenuView: View {
    var body: some View {
        VStack(alignment: .leading) {
            HStack {
                Image(systemName: "bed.double")
                    .foregroundColor(.gray)
                    .imageScale(.large)
                Text("Bedtime")
                    .foregroundColor(.gray)
                    .font(.headline)
            }
            .padding(.top, 100)
            HStack {
                Image(systemName: "tv")
                    .foregroundColor(.gray)
                    .imageScale(.large)
                Text("Watch TV")
                    .foregroundColor(.gray)
                    .font(.headline)
            }
            .padding(.top, 30)
            HStack {
                Image(systemName: "person")
                    .foregroundColor(.gray)
                    .imageScale(.large)
                Text("Logout")
                    .foregroundColor(.gray)
                    .font(.headline)
            }
            .padding(.top, 30)
            Spacer()
        }
        .padding()
        .frame(maxWidth: .infinity, alignment: .leading)
        .background(Color(red: 32/255, green: 32/255, blue: 32/255))
        .edgesIgnoringSafeArea(.all)
    }
}

struct MenuView_Previews: PreviewProvider {
    static var previews: some View {
        MenuView()
    }
}
