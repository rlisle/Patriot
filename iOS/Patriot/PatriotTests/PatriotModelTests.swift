//
//  PatriotModelTests.swift
//  PatriotTests
//
//  Created by Ron Lisle on 5/25/21.
//

import XCTest
@testable import Patriot

class PatriotModelTests: XCTestCase {

    var model: PatriotModel!
    
    override func setUpWithError() throws {
        model = PatriotModel(forTest: true)
    }

    
    func testNumberOfDevices() {
        XCTAssertEqual(model.devices.count, 21)
    }

    func testNumberOfRooms() {
        XCTAssertEqual(model.rooms.count, 4)
    }
    
    func testUpdateFavoritesList() {
        model.updateFavoritesList()
        XCTAssertEqual(model.favoritesList.count, 2)
    }

}
