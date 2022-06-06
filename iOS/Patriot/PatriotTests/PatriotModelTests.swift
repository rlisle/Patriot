//
//  PatriotModelTests.swift
//  PatriotTests
//
//  Created by Ron Lisle on 5/25/21.
//

import XCTest
@testable import Patriot

class MockSettingsStore: SettingsProviding {
    var mockBool: Bool?
    var mockInt: Int?
    var mockString: String?
    var mockStringArray: [String] = []
    var mockDeviceArray: [Device] = []
    
    func getBool(forKey: SettingsKey) -> Bool? {
        return mockBool
    }
    
    func set(_ bool: Bool?, forKey: SettingsKey) {
        mockBool = bool
    }
    
    func getInt(forKey: SettingsKey) -> Int? {
        return mockInt
    }
    
    func set(_ int: Int?, forKey: SettingsKey) {
        mockInt = int
    }
    
    func getString(forKey: SettingsKey) -> String? {
        return mockString
    }
    
    func set(_ string: String?, forKey: SettingsKey) {
        mockString = string
    }
    
    func getStringArray(forKey: SettingsKey) -> [String] {
        return mockStringArray
    }
    
    func set(_ array: [String], forKey: SettingsKey) {
        mockStringArray = array
    }
    
    func getDeviceArray(forKey: SettingsKey) -> [Device] {
        return mockDeviceArray
    }
    
    func set(_ array: [Device], forKey: SettingsKey) {
        mockDeviceArray = array
    }
    
    
}

class PatriotModelTests: XCTestCase {

    var model: PatriotModel!
    var store: SettingsProviding!
    var settings: Settings!
    
    override func setUpWithError() throws {
        model = PatriotModel(testMode: .on)
        store = MockSettingsStore()
        settings = Settings(store: store)
    }

    
    func testNumberOfDevices() {
        XCTAssertEqual(model.devices.count, 21)
    }

    func testNumberOfRooms() {
        XCTAssertEqual(model.rooms.count, 4)
    }
    
    func testUpdateFavoritesList() {
        model.updateFavoritesList(settings: settings)
        XCTAssertEqual(model.favoritesList.count, 2)
    }
}
