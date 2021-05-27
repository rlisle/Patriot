//
//  ContentView.swift
//  Patriot
//
//  Created by Ron Lisle on 5/25/21.
//

import SwiftUI

struct ContentView: View {
    
    @State private var needsLogin = true
    
    var body: some View {

        Button("Log Out") {
                    needsLogin = true
                }
                .sheet(isPresented: $needsLogin) {
                    LoginView(needsLogin: $needsLogin)
                }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
