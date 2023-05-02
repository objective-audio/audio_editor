//
//  SettingsHostingController.swift
//

import SwiftUI

class SettingsHostingController: NSHostingController<SettingsView<SettingsPresenter>> {
    init(lifetimeId: WindowLifetimeId) {
        let presenter = SettingsPresenter(lifetimeId: lifetimeId)
        let rootView = SettingsView<SettingsPresenter>(presenter: presenter)
        super.init(rootView: rootView)
    }

    @MainActor required dynamic init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    override func viewDidLoad() {
        super.viewDidLoad()
    }
}
