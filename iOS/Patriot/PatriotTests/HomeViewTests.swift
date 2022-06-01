//
//  HomeViewTests.swift
//  PatriotTests
//
//  Created by Ron Lisle on 5/31/22.
//


import XCTest
import SwiftUI
@testable import Patriot
import SnapshotTesting

class HomeViewTests: XCTestCase {
    
  let modelView = PatriotModel(forTest: true)

  func testHomeView() throws {
    let homeView = HomeView().environmentObject(PatriotModel(forTest: true))

    let view: UIView = UIHostingController(rootView: homeView).view
    assertSnapshot(matching: view, as: .image(size: view.intrinsicContentSize))
  }
}
