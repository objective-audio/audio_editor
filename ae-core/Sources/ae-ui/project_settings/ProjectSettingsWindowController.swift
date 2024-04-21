//
//  ProjectSettingsWindowController.swift
//

import AppKit
import ae_ui_objc

@objcMembers
public final class ProjectSettingsWindowController: NSWindowController {
    public var lifetimeId: ProjectLifetimeId { presenter.lifetimeId }
    private var presenter: ProjectSettingsWindowPresenter!

    public static func instantiate(lifetimeId: ProjectLifetimeId) -> ProjectSettingsWindowController
    {
        let storyboard = NSStoryboard(
            name: "ProjectSettingsWindow",
            bundle: Bundle.module)
        guard
            let windowController =
                storyboard.instantiateInitialController() as? ProjectSettingsWindowController
        else {
            fatalError()
        }
        windowController.presenter = .init(lifetimeId: lifetimeId)
        windowController.contentViewController = ProjectSettingsHostingController(
            lifetimeId: lifetimeId)
        return windowController
    }

    public override func windowDidLoad() {
        super.windowDidLoad()
        window?.title = "Project Settings"
    }
}

extension ProjectSettingsWindowController: NSWindowDelegate {
    public func windowShouldClose(_ sender: NSWindow) -> Bool {
        return presenter.shouldClose()
    }
}
