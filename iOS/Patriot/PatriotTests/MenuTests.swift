//
//  MenuViewTests.swift
//  PatriotTests
//
//  Created by Ron Lisle on 6/1/22.
//


import XCTest
import SwiftUI
//import SnapshotTesting
@testable import Patriot

class MenuViewTests: XCTestCase {
    
    var viewController: UIViewController!
    
    override func setUpWithError() throws {
        try super.setUpWithError()
        let modelView = PatriotModel(testMode: .on)
        let menuView = MenuView().environmentObject(modelView)
        viewController = UIHostingController(rootView: menuView)
    }
    
    func testMenuView() throws {
        assertSnapshot(matching: viewController, as: .image(on: .iPhoneX), record: false)
    }
}
