//
//  PatriotUITests.swift
//  PatriotUITests
//
//  XCUITests are used to verify navigation through the app.
//  UI appearance is tested using snapshot-testing in XCTests, not UI tests.
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

    func testAppNavigation() {
        // Launch app
        let homeTitle = app.navigationBars["Patriot"].staticTexts["Patriot"]
        XCTAssert(homeTitle.waitForExistence(timeout: 3))
                
        // Side Menu
        let patriotNavigationBar = app.navigationBars["Patriot"]
        let menuButton = patriotNavigationBar.buttons["Drag"]   // Is this the menu button? We should rename it
        menuButton.tap()    // Open side menu
        XCTAssert(app.buttons["Awake"].waitForExistence(timeout: 3))    // Note: this may pass even with menu closed ???
        menuButton.tap()    // Close side menu

        // Device Details
        let scrollViewsQuery = app.scrollViews
        let kitchenElementsQuery = scrollViewsQuery.otherElements.containing(.staticText, identifier:"Kitchen")
        let lightoffImage = kitchenElementsQuery.children(matching: .other).element(boundBy: 1).children(matching: .image).matching(identifier: "LightOff").element(boundBy: 1)
        lightoffImage/*@START_MENU_TOKEN@*/.press(forDuration: 1.4);/*[[".tap()",".press(forDuration: 1.4);"],[[[-1,1],[-1,0]]],[0]]@END_MENU_TOKEN@*/
        
        XCTAssert(app.staticTexts["Room: Kitchen"].exists)
        
        app.navigationBars["_TtGC7SwiftUI19UIHosting"].buttons["Patriot"].tap()

        //TODO: how to test that tapping on a device turns it on/off?
        
    }
}
