//
//  HomeView.swift
//  Patriot
//
//  Created by Ron Lisle on 5/25/21.
//

import SwiftUI

struct HomeView: View {

    @State var showMenu = false
    @EnvironmentObject var model: PatriotModel

    init() {
        //Use this if NavigationBarTitle is with Large Font
        //UINavigationBar.appearance().largeTitleTextAttributes = [.foregroundColor: UIColor.white]

        //Use this if NavigationBarTitle is with displayMode = .inline
        UINavigationBar.appearance().titleTextAttributes = [.foregroundColor: UIColor.white]
    }
    
    var body: some View {
        
        let dragToClose = DragGesture()
            .onEnded {
                if $0.translation.width < -100 {
                    withAnimation {
                        self.showMenu = false
                    }
                }
            }
        
        NavigationView {
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
                .gesture(dragToClose)
            }

            .navigationBarTitle("Patriot")
            .navigationBarTitleDisplayMode(.inline)
            .foregroundColor(.white)
            .toolbar {
                ToolbarItemGroup(placement: .navigationBarLeading) {
                    SideMenuButton(showMenu: $showMenu)
                }
                ToolbarItemGroup(placement: .navigationBarTrailing) {
                    SleepingButton()
                    PartOfDayButton()
                }
            }
            
        }//navView
        // This fixes the layout constraint warnings
        .navigationViewStyle(StackNavigationViewStyle())

    }//body
}

struct HomeView_Previews: PreviewProvider {
    static var previews: some View {
        HomeView()
            .environmentObject(PatriotModel(forTest: true))
    }
}
