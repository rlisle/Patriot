//
//  HomeView.swift
//  Patriot
//
//  Created by Ron Lisle on 5/25/21.
//

import SwiftUI

struct HomeView: View {

    @State var showMenu = false
    
    init() {
        //Use this if NavigationBarTitle is with Large Font
        //UINavigationBar.appearance().largeTitleTextAttributes = [.foregroundColor: UIColor.white]

        //Use this if NavigationBarTitle is with displayMode = .inline
        UINavigationBar.appearance().titleTextAttributes = [.foregroundColor: UIColor.white]
    }
    
    var body: some View {
        
        let drag = DragGesture()
            .onEnded {
                if $0.translation.width < -100 {
                    withAnimation {
                        self.showMenu = false
                    }
                }
            }
        
        return NavigationView {
            GeometryReader { geometry in
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

            .navigationBarTitle("Patriot").foregroundColor(.white)
            .toolbar {
                ToolbarItemGroup(placement: .navigationBarLeading) {

                    Button(action: {
                        withAnimation {
                            self.showMenu.toggle()
                        }
                    }) {
                        Image(systemName: "line.horizontal.3")
                            .imageScale(.large)
                    }.foregroundColor(.white)

                }
                ToolbarItemGroup(placement: .navigationBarTrailing) {
                    Button(action: {
                        withAnimation {
                            print("Sleeping")
                            //TODO: Cycle through states
                        }
                    }) {
                        //TODO: Display current sleeping state
                        SleepView(sleeping: .Awake)
                    }.foregroundColor(.white)

                    Button(action: {
                        withAnimation {
                            print("PartOfDay")
                        }
                    }) {
                        Image(systemName: "sun.and.horizon")
                            .imageScale(.large)
                    }.foregroundColor(.white)

                }

            }
            
        }//navView
        // This fixes the layout constraint warnings
        .navigationViewStyle(StackNavigationViewStyle())

    }//body
}

struct SleepView: View {

    @State var sleeping: Sleeping
    
    var body: some View {
        Image(systemName: sleepIcon())
            .imageScale(.large)
    }
    
    func sleepIcon() -> String {
        return "bed.double.circle"
    }
}

struct HomeView_Previews: PreviewProvider {
    static var previews: some View {
        HomeView()
            .environmentObject(PatriotModel(
                                devices: [
                                    Device(name: "Light", type: .Light, percent: 0, room: "Office", isFavorite: true),
                                    Device(name: "Switch", type: .Switch, percent: 0, room: "Office", isFavorite: false),
                                    Device(name: "Curtain", type: .Curtain, percent: 0, room: "Office", isFavorite: true),
                                    Device(name: "Light2", type: .Light, percent: 100, room: "Office", isFavorite: false),
                                    Device(name: "Switch2", type: .Switch, percent: 0, room: "Bedroom", isFavorite: false),
                                    Device(name: "Light3", type: .Light, percent: 0, room: "Bedroom", isFavorite: false),
                                    Device(name: "Light4", type: .Light, percent: 100, room: "Bedroom", isFavorite: false)
                                ]))
    }
}
