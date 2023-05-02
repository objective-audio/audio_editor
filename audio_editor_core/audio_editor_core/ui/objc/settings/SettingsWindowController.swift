//
//  SettingsWindowController.swift
//

import AppKit

@objcMembers
public final class SettingsWindowController: NSWindowController {
    public var lifetimeId: WindowLifetimeId { self.presenter.lifetimeId }
    private var presenter: SettingsWindowPresenter!

    public static func instantiate(lifetimeId: WindowLifetimeId) -> SettingsWindowController {
        let storyboard = NSStoryboard(name: "SettingsWindow",
                                      bundle: Bundle(for: Self.self))
        guard let windowController = storyboard.instantiateInitialController() as? SettingsWindowController else {
            fatalError()
        }
        windowController.presenter = .init(lifetimeId: lifetimeId)
        windowController.contentViewController = SettingsHostingController(lifetimeId: lifetimeId)
        return windowController
    }

    public override func windowDidLoad() {
        super.windowDidLoad()
        self.window?.title = "Project Settings"
    }
}

extension SettingsWindowController: NSWindowDelegate {
    public func windowShouldClose(_ sender: NSWindow) -> Bool {
        return self.presenter.shouldClose()
    }
}
