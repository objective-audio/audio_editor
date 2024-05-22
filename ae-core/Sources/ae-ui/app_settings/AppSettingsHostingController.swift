//
//  AppSettingsHostingController.swift
//

import SwiftUI

class AppSettingsHostingController: NSHostingController<AppSettingsView<AppSettingsPresenter>> {
    init() {
        let rootView = AppSettingsView<AppSettingsPresenter>(presenter: AppSettingsPresenter())
        super.init(rootView: rootView)
    }

    @MainActor required dynamic init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    override func viewDidLoad() {
        super.viewDidLoad()
    }
}
