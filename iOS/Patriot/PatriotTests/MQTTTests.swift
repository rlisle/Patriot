//
//  MQTTTests.swift
//  PatriotTests
//
//  Created by Ron Lisle on 5/21/22.
//

import XCTest
@testable import Patriot

class MQTTTests: XCTestCase {

    var mqtt: MQTTManager!
    
    override func setUpWithError() throws {
        mqtt = MQTTManager(forTest: true)
    }

    func testInstantiation() throws {
        XCTAssertNotNil(mqtt)
    }
}
