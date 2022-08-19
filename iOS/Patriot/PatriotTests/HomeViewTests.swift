//
//  HomeViewTests.swift
//  PatriotTests
//
//  Created by Ron Lisle on 5/31/22.
//


import XCTest
import SwiftUI
@testable import Patriot

class HomeViewTests: XCTestCase {
    
    func testHomeView() throws {
        let modelView = PatriotModel(testMode: .on)
        let homeView = HomeView().environmentObject(modelView)
        let viewController = UIHostingController(rootView: homeView)
        assertSnapshot(matching: viewController, as: .image(on: .iPhone8), record: false)
    }
    
    func testHomeViewWithChangedLights() throws {
        let modelView = PatriotModel(testMode: .on)
        modelView.devices[0].percent = 100
        let homeView = HomeView().environmentObject(modelView)
        let viewController = UIHostingController(rootView: homeView)
        assertSnapshot(matching: viewController, as: .image(on: .iPhone8), record: false)
    }
}

extension SwiftUI.View {
    func toVC() -> UIViewController {
        let vc = UIHostingController(rootView: self)
        vc.view.frame = UIScreen.main.bounds
        return vc
    }
}
