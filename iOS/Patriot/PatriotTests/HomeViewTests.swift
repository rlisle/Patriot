//
//  HomeViewTests.swift
//  PatriotTests
//
//  Created by Ron Lisle on 5/31/22.
//


import XCTest
import SwiftUI
import SnapshotTesting
@testable import Patriot

class HomeViewTests: XCTestCase {
    
    func testHomeView() throws {
        let modelView = PatriotModel(forTest: true)
        let homeView = HomeView().environmentObject(modelView)
        let viewController = UIHostingController(rootView: homeView)
        assertSnapshot(matching: viewController, as: .image(on: .iPhoneX))
    }
    
    func testHomeViewWithChangedLights() throws {
        let modelView = PatriotModel(forTest: true)
        modelView.devices[0].percent = 100
        let homeView = HomeView().environmentObject(modelView)
        let viewController = UIHostingController(rootView: homeView)
        assertSnapshot(matching: viewController, as: .image(on: .iPhoneX))
    }
}
