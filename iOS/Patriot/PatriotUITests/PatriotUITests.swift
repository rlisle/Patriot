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
    var patriotNavigationBar: XCUIElement!
    var menuButton: XCUIElement!

    override func setUpWithError() throws {
        continueAfterFailure = false
        app = XCUIApplication()
        app.launch()
        patriotNavigationBar = app.navigationBars["Patriot"]
        menuButton = patriotNavigationBar.buttons["Drag"]
    }

    func testAppNavigation() {
        
        // Launch app
        verifyHomeScreen()
                
        // Side Menu
        openSideMenu()
        verifySideMenuOpen()
        closeSideMenu()

        // Device Details
        openKitchenDetails()
        verifyKitchenDetails()
        closeKitchenDetails()

        verifyHomeScreen()

        //TODO: how to test that tapping on a device turns it on/off?
    }
    
    func verifyHomeScreen() {
        let homeTitle = app.navigationBars["Patriot"].staticTexts["Patriot"]
        XCTAssert(homeTitle.waitForExistence(timeout: 3))
    }
    
    func openSideMenu() {
        menuButton.tap()    // Open side menu
    }
    
    func closeSideMenu() {
        menuButton.tap()    // Close side menu
    }
    
    func verifySideMenuOpen() {
        XCTAssert(app.buttons["Awake"].waitForExistence(timeout: 3))
    }
    
    func openKitchenDetails() {
        // Open one of the Kitchen lights that are currently off (test will fail if all are on)
        let scrollViewsQuery = app.scrollViews
        let kitchenElementsQuery = scrollViewsQuery.otherElements.containing(.staticText, identifier:"Kitchen")
        let lightoffImage = kitchenElementsQuery.children(matching: .other).element(boundBy: 1).children(matching: .image).matching(identifier: "LightOff").element(boundBy: 1)
        lightoffImage/*@START_MENU_TOKEN@*/.press(forDuration: 1.4);/*[[".tap()",".press(forDuration: 1.4);"],[[[-1,1],[-1,0]]],[0]]@END_MENU_TOKEN@*/
    }
    
    func verifyKitchenDetails() {
        XCTAssert(app.staticTexts["Brightness"].exists)
    }
    
    func closeKitchenDetails() {
        app.navigationBars["_TtGC7SwiftUI19UIHosting"].buttons["Patriot"].tap()
    }
    
}
