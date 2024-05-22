//
//  AppSettingsWindowController.swift
//

import AppKit
import ae_ui_objc

@objcMembers
public final class AppSettingsWindowController: NSWindowController {
    public var lifetimeId: AppSettingsLifetimeId { presenter.lifetimeId }
    private var presenter: AppSettingsWindowPresenter!

    public static func instantiate(lifetimeId: AppSettingsLifetimeId) -> AppSettingsWindowController
    {
        let storyboard = NSStoryboard(
            name: "AppSettingsWindow",
            bundle: Bundle.module)
        guard
            let windowController =
                storyboard.instantiateInitialController() as? AppSettingsWindowController
        else {
            fatalError()
        }
        windowController.presenter = .init(lifetimeId: lifetimeId)
        windowController.contentViewController = AppSettingsHostingController()
        return windowController
    }

    public override func windowDidLoad() {
        super.windowDidLoad()
        window?.title = "App Settings"
    }
}

extension AppSettingsWindowController: NSWindowDelegate {
    public func windowShouldClose(_ sender: NSWindow) -> Bool {
        return presenter.shouldClose()
    }
}
