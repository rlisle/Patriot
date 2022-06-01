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

    var viewController: UIViewController!
    
    override func setUpWithError() throws {
        try super.setUpWithError()
        let modelView = PatriotModel(forTest: true)
        let homeView = HomeView().environmentObject(modelView)
        viewController = UIHostingController(rootView: homeView)
    }

  func testHomeView() throws {
    assertSnapshot(matching: viewController, as: .image(on: .iPhoneX))
  }
}
