//
//  PatriotUITests.swift
//  PatriotUITests
//
//  Created by Ron Lisle on 6/1/22.
//

import XCTest

class PatriotUITests: XCTestCase {

    var app: XCUIApplication!
    
    override func setUpWithError() throws {
        continueAfterFailure = false
        app = XCUIApplication()
        app.launch()
    }

//    func testRecord() throws {
//
//        let app = XCUIApplication()
//        app.navigationBars["Patriot"].staticTexts["Patriot"].tap()
//
//        let scrollViewsQuery = app.scrollViews
//        let bedroomStaticText = scrollViewsQuery.otherElements.containing(.staticText, identifier:"Kitchen").children(matching: .staticText)["Bedroom"]
//        bedroomStaticText.tap()
//        scrollViewsQuery.otherElements.staticTexts["Kitchen"].tap()
//        bedroomStaticText.tap()
//
//    }

    func testHomeScreenLaunches() {
        let homeTitle = app.navigationBars["Patriot"].staticTexts["Patriot"]
        XCTAssert(homeTitle.waitForExistence(timeout: 3))
    }
    
    func testSideMenu() {
        let homeTitle = app.navigationBars["Patriot"].staticTexts["Patriot"]
        XCTAssert(homeTitle.waitForExistence(timeout: 3))
        let patriotNavigationBar = app.navigationBars["Patriot"]
        let menuButton = patriotNavigationBar.buttons["Drag"]   // Is this the menu button? We should rename it
        menuButton.tap()
        XCTAssert(app.buttons["Awake"].waitForExistence(timeout: 3))
    }
    
    func testDetailView() {
        let homeTitle = app.navigationBars["Patriot"].staticTexts["Patriot"]
        homeTitle.waitForExistence(timeout: 3)
        let scrollViewsQuery = app.scrollViews
        let kitchenElementsQuery = scrollViewsQuery.otherElements.containing(.staticText, identifier:"Kitchen")
        let lightoffImage = kitchenElementsQuery.children(matching: .other).element(boundBy: 1).children(matching: .image).matching(identifier: "LightOff").element(boundBy: 1)
        lightoffImage/*@START_MENU_TOKEN@*/.press(forDuration: 1.4);/*[[".tap()",".press(forDuration: 1.4);"],[[[-1,1],[-1,0]]],[0]]@END_MENU_TOKEN@*/
        XCTAssert(app.staticTexts["Room: Kitchen"].exists)
        XCTAssert(app.staticTexts["Sink"].exists)
    }
    
//    func testPatriot() throws {
//        let app = XCUIApplication()
//        app.launch()
//        let patriotNavigationBar = app.navigationBars["Patriot"]
//        let dragButton = patriotNavigationBar.buttons["Drag"]   // Is this the menu button?
//
//        // Wait for app to launch
//
//        // Verify Home screen is displayed
//
//        // Navigate to side Menu
//
//        // Verify side Menu is displayed
//
//        // Return to Home screen
//
//        // Navigate to Device Detail screen
//
//
//        dragButton.tap()
//        app.buttons["Awake"].tap()
//        dragButton.tap()
//
//        let scrollViewsQuery = app.scrollViews
//        let kitchenElementsQuery = scrollViewsQuery.otherElements.containing(.staticText, identifier:"Kitchen")
//        let lightoffImage = kitchenElementsQuery.children(matching: .other).element(boundBy: 1).children(matching: .image).matching(identifier: "LightOff").element(boundBy: 1)
//        lightoffImage.tap()
//
//        let elementsQuery = scrollViewsQuery.otherElements
//        elementsQuery.images["LightOn"].tap()
//        lightoffImage/*@START_MENU_TOKEN@*/.press(forDuration: 1.4);/*[[".tap()",".press(forDuration: 1.4);"],[[[-1,1],[-1,0]]],[0]]@END_MENU_TOKEN@*/
//        app.staticTexts["Room: Kitchen"].tap()
//        app.staticTexts["Sink"].tap()
//        app.navigationBars["_TtGC7SwiftUI19UIHosting"].buttons["Patriot"].tap()
//        patriotNavigationBar.buttons["Walk"].tap()
//        patriotNavigationBar.buttons["Brightness Lower"].tap()
//        kitchenElementsQuery.children(matching: .staticText)["Bedroom"].tap()
//        elementsQuery.staticTexts["Kitchen"].tap()
//
//
//    }
}
