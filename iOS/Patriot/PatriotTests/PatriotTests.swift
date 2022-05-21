//
//  PatriotTests.swift
//  PatriotTests
//
//  Created by Ron Lisle on 5/25/21.
//

import XCTest
@testable import Patriot

class PatriotTests: XCTestCase {

    var model: PatriotModel!
    
    override func setUpWithError() throws {
        model = PatriotModel(forTest: true)
    }

    func testInstantiation() {
        XCTAssertNotNil(model)
    }
    
    func testNumberOfFavorites() {
        XCTAssertEqual(model.devices.filter { $0.isFavorite == true }.count, 2)
    }
    
    func testNumberOfRooms() {
        XCTAssertEqual(model.rooms.count, 4)
    }

}
