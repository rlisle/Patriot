//
//  HomeView.swift
//  Patriot
//
//  Created by Ron Lisle on 5/25/21.
//

import SwiftUI

struct HomeView: View {

    let sideMenuWidth: CGFloat = 200
    
    @State var showMenu = false
    @State var showDetails = false
    @EnvironmentObject var model: PatriotModel
    @EnvironmentObject var appDelegate: AppDelegate
    @EnvironmentObject var sceneDelegate: SceneDelegate

    init() {
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
            ZStack(alignment: .leading) {
                MainView(showMenu: self.$showMenu)
                    .offset(x: self.showMenu ? sideMenuWidth : 0)
                    .disabled(self.showMenu ? true : false)

                if self.showMenu {
                    MenuView()
                        .frame(width: sideMenuWidth)
                }
            }
            .background(Color.black)
            .gesture(dragToClose)

            .navigationBarTitle("Patriot")
            .navigationBarTitleDisplayMode(.inline)
            .foregroundColor(.white)
            .toolbar {
                ToolbarItemGroup(placement: .navigationBarLeading) {
                    SideMenuButton(showMenu: $showMenu)
                }
                ToolbarItemGroup(placement: .navigationBarTrailing) {
                    SleepingButton(sleeping: model.sleeping)
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
