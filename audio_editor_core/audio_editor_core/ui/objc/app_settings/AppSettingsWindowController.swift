//
//  AppSettingsWindowController.swift
//

import AppKit

@objcMembers
public final class AppSettingsWindowController: NSWindowController {
    public var lifetimeId: AppSettingsLifetimeId { self.presenter.lifetimeId }
    private var presenter: AppSettingsWindowPresenter!

    public static func instantiate(lifetimeId: AppSettingsLifetimeId) -> AppSettingsWindowController {
        let storyboard = NSStoryboard(name: "AppSettingsWindow",
                                      bundle: Bundle(for: Self.self))
        guard let windowController =
                storyboard.instantiateInitialController() as? AppSettingsWindowController else {
            fatalError()
        }
        windowController.presenter = .init(lifetimeId: lifetimeId)
        windowController.contentViewController = AppSettingsHostingController()
        return windowController
    }

    public override func windowDidLoad() {
        super.windowDidLoad()
        self.window?.title = "App Settings"
    }
}

extension AppSettingsWindowController: NSWindowDelegate {
    public func windowShouldClose(_ sender: NSWindow) -> Bool {
        return self.presenter.shouldClose()
    }
}
