//
//  AssertSnapshot.swift
//  PatriotTests
//
//  Created by Ron Lisle on 6/22/22.
//  Copied from Morten Bek Ditlevsen per https://github.com/pointfreeco/swift-snapshot-testing/discussions/553
//
// 8/12/22 Investigating running on Xcode Cloud

import Foundation
import SnapshotTesting
import SwiftUI
import XCTest

public func assertSnapshot<Value, Format>(
    matching value: @autoclosure () throws -> Value,
    as snapshotting: Snapshotting<Value, Format>,
    named name: String? = nil,
    record recording: Bool = false,
    timeout: TimeInterval = 5,
    file: StaticString = #file,
    testName: String = #function,
    line: UInt = #line
) {
    // Set overrideRecording to true to re-record all snapshot test images
    // This is easier than editing every snapshot test file
    let overrideRecording = false
    
    let isCI = ProcessInfo.processInfo.environment["CI"] == "TRUE"
    var sourceRoot = URL(fileURLWithPath: ProcessInfo.processInfo.environment["SOURCE_ROOT"]!,
                         isDirectory: true)
    if isCI {
        sourceRoot = URL(fileURLWithPath: "/Volumes/workspace/repository")
    }

    let fileUrl = URL(fileURLWithPath: "\(file)", isDirectory: false)
    let fileName = fileUrl.deletingPathExtension().lastPathComponent

    let absoluteSourceTestPath = fileUrl
        .deletingLastPathComponent()
        .appendingPathComponent("__Snapshots__")
        .appendingPathComponent(fileName)
    var components = absoluteSourceTestPath.pathComponents
    let sourceRootComponents = sourceRoot.pathComponents
    // Remove sourceRoot from the filePath
    for component in sourceRootComponents {
        if components.first == component {
            components = Array(components.dropFirst())
        } else {
            XCTFail("Test file '\(file)' does not share a prefix path with sourceRoot '\(sourceRoot)'")
            return
        }
    }
    // At this point components should have sourceRootComponents removed
    // But in the case of Patriot, there are still '/iOS/Patriot/'
    // Our repo starts 2 dirs above the project source_root, so remove those also
    // There's probably a better, more generic way to do this.
    if isCI {
        components = Array(components.dropFirst())
        components = Array(components.dropFirst())
    }
    
    var snapshotDirectoryUrl = sourceRoot
    if isCI {
        snapshotDirectoryUrl = snapshotDirectoryUrl.appendingPathComponent("ci_scripts")
    }
    snapshotDirectoryUrl = snapshotDirectoryUrl.appendingPathComponent("Artifacts")
    for component in components {
        snapshotDirectoryUrl = snapshotDirectoryUrl.appendingPathComponent(component)
    }

    let failure = verifySnapshot(
        matching: try value(),
        as: snapshotting,
        named: name,
        record: recording || overrideRecording,
        snapshotDirectory: snapshotDirectoryUrl.path,
        timeout: timeout,
        file: file,
        testName: testName
    )
    guard let message = failure else { return }
    XCTFail("\(message) snap: \(snapshotDirectoryUrl)", file: file, line: line)
}

extension SwiftUI.View {
    func toVC() -> UIViewController {
        let vc = UIHostingController(rootView: self)
        vc.view.frame = UIScreen.main.bounds
        return vc
    }
}
