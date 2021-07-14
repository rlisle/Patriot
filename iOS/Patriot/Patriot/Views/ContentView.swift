//
//  ContentView.swift
//  Patriot
//
//  Created by Ron Lisle on 5/25/21.
//

import SwiftUI

struct ContentView: View {

//    @EnvironmentObject var devices: DevicesManager
    
    @State var showMenu = false
    
    var body: some View {
        
        let drag = DragGesture()
            .onEnded {
                if $0.translation.width < -100 {
                    withAnimation {
                        self.showMenu = false
                    }
                }
            }
        
        return GeometryReader { geometry in
            ZStack(alignment: .leading) {
                MainView(showMenu: self.$showMenu)
                    .frame(width: geometry.size.width, height: geometry.size.height)
                    .offset(x: self.showMenu ? geometry.size.width/2 : 0)
                    .disabled(self.showMenu ? true : false)

                if self.showMenu {
                    MenuView()
                        .frame(width: geometry.size.width/2)
                    
                }
            }
            .gesture(drag)
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
            .environmentObject(DevicesManager(
                                devices: [
                                    Device(name: "Light", type: .Light),
                                    Device(name: "Switch", type: .Switch),
                                    Device(name: "Curtain", type: .Curtain)
                                ]))
    }
}
