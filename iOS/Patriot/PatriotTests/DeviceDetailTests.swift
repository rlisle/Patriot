//
//  DeviceDetailTests.swift
//  PatriotTests
//
//  Created by Ron Lisle on 5/31/22.
//


import XCTest
import SwiftUI
import SnapshotTesting
@testable import Patriot

class DeviceDetailTests: XCTestCase {
    
    var viewController: UIViewController!
    
    override func setUpWithError() throws {
        try super.setUpWithError()
        let modelView = PatriotModel(testMode: .on)
        let deviceDetailView = DeviceDetailView().environmentObject(modelView)
        viewController = UIHostingController(rootView: deviceDetailView)
    }
    
    func testDeviceDetailView() throws {
        //isRecording = true
        assertSnapshot(matching: viewController, as: .image(on: .iPhoneX), record: false)
    }
}
