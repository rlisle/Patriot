//
//  AssertSnapshot.swift
//  PatriotTests
//
//  Created by Ron Lisle on 6/22/22.
//  Copied from Morten Bek Ditlevsen per https://github.com/pointfreeco/swift-snapshot-testing/discussions/553
//
//  Currently need to add CI $(CI) and SOURCE_ROOT $(SOURCE_ROOT) to scheme run environment settings
//
// 8/12/22 Investigating running on Xcode Cloud

import Foundation
import SnapshotTesting
import XCTest

// Only files in ci_scripts are available to Xcode Cloud tests

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
    
    print("DEBUG: file = \(file)")  // /Users/ronlisle/GitRepos/Patriot/iOS/Patriot/PatriotTests/MenuTests.swift

    let isCI = ProcessInfo.processInfo.environment["CI"] == "TRUE"
//    let envKey = isCI ? "CI_WORKSPACE" : "SOURCE_ROOT"
//    let sourceRoot = URL(fileURLWithPath: ProcessInfo.processInfo.environment[envKey]!,
//                         isDirectory: true)
//
//    print("DEBUG: sourceRoot = \(sourceRoot)")  // local: file:///Users/ronlisle/GitRepos/Patriot/iOS/Patriot/
//
//    let fileUrl = URL(fileURLWithPath: "\(file)", isDirectory: false)
//    let fileName = fileUrl.deletingPathExtension().lastPathComponent
//
//    let absoluteSourceTestPath = fileUrl
//        .deletingLastPathComponent()
//        .appendingPathComponent("__Snapshots__")
//        .appendingPathComponent(fileName)
//    var components = absoluteSourceTestPath.pathComponents
//    let sourceRootComponents = sourceRoot.pathComponents
//    for component in sourceRootComponents {
//        if components.first == component {  // This fails is component appears multiple times (eg. patriot/Snapshots/patriot)
//            components = Array(components.dropFirst())
//        } else {
//            XCTFail("Test file does not share a prefix path with SOURCE_ROOT")
//            return
//        }
//    }
//
//    var snapshotDirectoryUrl = sourceRoot
//    if isCI {
//        snapshotDirectoryUrl = snapshotDirectoryUrl.appendingPathComponent("ci_scripts")
//    }
//    snapshotDirectoryUrl = snapshotDirectoryUrl.appendingPathComponent("Snapshots")
//    for component in components {
//        snapshotDirectoryUrl = snapshotDirectoryUrl.appendingPathComponent(component)
//    }
//    print("DEBUG: verifySnapshot directory: \(snapshotDirectoryUrl.path), file: \(file)")

    var snapshotDirectory = "/Users/ronlisle/GitRepos/Patriot/iOS/Patriot/Snapshots/PatriotTests/__Snapshots__/MenuTests"
    if isCI {
//        snapshotDirectory = "/Volumes/workspace/repository/Snapshots/PatriotTests/__Snapshots__/MenuTests"
        snapshotDirectory = "Snapshots/PatriotTests/__Snapshots__/MenuTests"
    }
    print("DEBUG: verifySnapshot directory: \(snapshotDirectory), file: \(file)")
    
    let failure = verifySnapshot(
        matching: try value(),
        as: snapshotting,
        named: name,
        record: recording,
//        snapshotDirectory: snapshotDirectoryUrl.path,
        snapshotDirectory: snapshotDirectory,
        timeout: timeout,
        file: file,
        testName: testName
    )
    guard let message = failure else { return }
//    XCTFail("\(message) snap: \(snapshotDirectoryUrl)", file: file, line: line)
    XCTFail("\(message) snap: \(snapshotDirectory)", file: file, line: line)
}


