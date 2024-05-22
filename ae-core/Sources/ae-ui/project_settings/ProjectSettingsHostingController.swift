//
//  ProjectSettingsHostingController.swift
//

import SwiftUI
import ae_ui_objc

class ProjectSettingsHostingController: NSHostingController<
    ProjectSettingsView<ProjectSettingsPresenter>
>
{
    init(lifetimeId: ProjectLifetimeId) {
        let presenter = ProjectSettingsPresenter(lifetimeId: lifetimeId)
        let rootView = ProjectSettingsView<ProjectSettingsPresenter>(presenter: presenter)
        super.init(rootView: rootView)
    }

    @MainActor required dynamic init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    override func viewDidLoad() {
        super.viewDidLoad()
    }
}
