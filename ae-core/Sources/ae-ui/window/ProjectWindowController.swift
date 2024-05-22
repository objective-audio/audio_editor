import Foundation
import ae_ui_objc

class ProjectWindowController: NSWindowController {
    var lifetimeId: ProjectLifetimeId { presenter.lifetimeId }

    private var presenter: ProjectWindowPresenter!

    static func instantiate(lifetimeId: ProjectLifetimeId) -> ProjectWindowController {
        let storyboard = NSStoryboard(name: "Window", bundle: Bundle.module)

        guard
            let windowController = storyboard.instantiateInitialController()
                as? ProjectWindowController
        else {
            fatalError()
        }

        windowController.setup(lifetimeId: lifetimeId)

        return windowController
    }

    func setup(lifetimeId: ProjectLifetimeId) {
        presenter = .init(lifetimeId: lifetimeId)

        if let window {
            window.title = presenter.title

            if let autoSaveName = presenter.autoSaveName {
                window.setFrameAutosaveName(autoSaveName)
            }

            if let url = presenter.representedUrl {
                window.representedURL = url
            }
        }

        presenter.observe { [weak self] in
            guard let content = self?.contentViewController as? AEMetalViewController else {
                // assertion_failure_if_not_test();
                return
            }

            content.setup(with: lifetimeId)
        }
    }
}

extension ProjectWindowController: NSWindowDelegate {
    func windowShouldClose(_ sender: NSWindow) -> Bool {
        presenter.shouldClose
    }
}
