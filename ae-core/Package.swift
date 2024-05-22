// swift-tools-version: 5.9
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "ae-core",
    platforms: [.macOS(.v14)],
    products: [
        .library(
            name: "ae-core",
            targets: ["ae-ui"]),
    ],
    dependencies: [
        .package(url: "https://github.com/objective-audio/audio_engine.git", branch: "master"),
        .package(url: "https://github.com/objective-audio/ui.git", branch: "master"),
        .package(url: "https://github.com/objective-audio/db.git", branch: "master"),
    ],
    targets: [
        .target(
            name: "ae-swift-bundle",
            resources: [.process("Resources")]
        ),
        .target(
            name: "ae-bundle",
            dependencies: [
                "ae-swift-bundle"
            ]
        ),
        .target(
            name: "ae-core-cpp-types",
            dependencies: [
                .product(name: "ui", package: "ui")
            ]
        ),
        .target(
            name: "ae-core-objc-types",
            dependencies: [
                "ae-core-cpp-types"
            ]
        ),
        .target(
            name: "ae-core",
            dependencies: [
                .product(name: "audio", package: "audio_engine"),
                .product(name: "ui", package: "ui"),
                .product(name: "db", package: "db"),
                "ae-bundle",
                "ae-core-objc-types"
            ]
        ),
        .target(
            name: "ae-ui-cpp",
            dependencies: [
                "ae-core"
            ]
        ),
        .target(
            name: "ae-ui-objc",
            dependencies: [
                "ae-ui-cpp"
            ]
        ),
        .target(
            name: "ae-ui",
            dependencies: [
                "ae-ui-objc"
            ]
        ),
        .testTarget(
            name: "ae-core-tests",
            dependencies: [
                "ae-core"
            ]
        ),
        .testTarget(
            name: "ae-bundle-tests",
            dependencies: ["ae-swift-bundle"]
        )
    ],
    cLanguageStandard: .gnu18,
    cxxLanguageStandard: .gnucxx2b
)
