//
//  ProjectSettingsHostingController.swift
//

import SwiftUI

class ProjectSettingsHostingController: NSHostingController<ProjectSettingsView<ProjectSettingsPresenter>> {
    init(lifetimeId: WindowLifetimeId) {
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
