//
//  HomeView.swift
//  Patriot
//
//  Created by Ron Lisle on 5/25/21.
//

import SwiftUI

struct HomeView: View {

    let sideMenuWidth: CGFloat = 200
        
    @EnvironmentObject var model: PatriotModel
    @EnvironmentObject var appDelegate: AppDelegate
    @EnvironmentObject var sceneDelegate: SceneDelegate
    
    var body: some View {
        
        let dragToClose = DragGesture()
            .onEnded {
                if $0.translation.width < -100 {
                    withAnimation {
                        model.showingMenu = false
                    }
                }
            }
        
        NavigationView {
            ZStack(alignment: .leading) {
                NavigationLink("", isActive: $model.showingDetails, destination: {
                    DeviceDetailView()
                })
                
                if model.devices == [] {
                    ZStack {
                        VStack {
                            Spacer()
                            HStack {
                                Spacer()
                                ProgressView("Searching for devices...")
                                Spacer()
                            }
                            Spacer()
                        }
                    }
                    .background(Color("BackgroundColor"))
                } else {
                    MainView()
                        .offset(x: model.showingMenu ? sideMenuWidth : 0)
                        .disabled(model.showingMenu ? true : false)
                }
                
                if model.showingMenu {
                    MenuView()
                        .transition(.move(edge: .leading))
                        .frame(width: sideMenuWidth)
                }
            }
            .background(Color("BackgroundColor"))
            .gesture(dragToClose)

            .navigationBarTitle("Patriot")
            .navigationBarTitleDisplayMode(.inline)
            .foregroundColor(Color("TextColor"))
            .toolbar {
                ToolbarItemGroup(placement: .navigationBarLeading) {
                    SideMenuButton(showMenu: $model.showingMenu)
                }
                ToolbarItemGroup(placement: .navigationBarTrailing) {
                    MQTTButton(isConnected: model.isConnected)
                        .padding(.horizontal, -8)
                    SleepingButton(sleeping: model.sleeping)
                        .padding(.horizontal, -8)
                    PartOfDayButton()
                        .padding(.horizontal, -8)
                }
            }
            
        }
        .accentColor(.black)
        
        
        // This fixes the layout constraint warnings
        .navigationViewStyle(StackNavigationViewStyle())
        
        .onAppear {
             let appearance = UINavigationBarAppearance()
             appearance.backgroundEffect = UIBlurEffect(style: .systemUltraThinMaterial)
             appearance.backgroundColor = UIColor(Color("BackgroundColor"))
            appearance.titleTextAttributes = [.foregroundColor: UIColor(Color("HeadingBackground"))]
             UINavigationBar.appearance().standardAppearance = appearance
             UINavigationBar.appearance().scrollEdgeAppearance = appearance
         }
    }
}

struct HomeView_Previews: PreviewProvider {
    static var previews: some View {
        Group {

            // List available sims: xcrun simctl list devicetypes
            HomeView()
                .environmentObject(PatriotModel(testMode: .on))
                .previewDevice(PreviewDevice(rawValue: "iPhone 11 Pro"))                .previewDisplayName("With Devices")

            HomeView()
                .environmentObject(PatriotModel(testMode: .noDevices))
                .previewDevice(PreviewDevice(rawValue: "iPhone 11 Pro"))
                .previewDisplayName("No Devices")

        }
    }
}
