//
//  ProjectSettingsWindowController.swift
//

import AppKit

@objcMembers
public final class ProjectSettingsWindowController: NSWindowController {
    public var lifetimeId: ProjectLifetimeId { self.presenter.lifetimeId }
    private var presenter: ProjectSettingsWindowPresenter!

    public static func instantiate(lifetimeId: ProjectLifetimeId) -> ProjectSettingsWindowController {
        let storyboard = NSStoryboard(name: "ProjectSettingsWindow",
                                      bundle: Bundle(for: Self.self))
        guard let windowController =
                storyboard.instantiateInitialController() as? ProjectSettingsWindowController else {
            fatalError()
        }
        windowController.presenter = .init(lifetimeId: lifetimeId)
        windowController.contentViewController = ProjectSettingsHostingController(lifetimeId: lifetimeId)
        return windowController
    }

    public override func windowDidLoad() {
        super.windowDidLoad()
        self.window?.title = "Project Settings"
    }
}

extension ProjectSettingsWindowController: NSWindowDelegate {
    public func windowShouldClose(_ sender: NSWindow) -> Bool {
        return self.presenter.shouldClose()
    }
}
