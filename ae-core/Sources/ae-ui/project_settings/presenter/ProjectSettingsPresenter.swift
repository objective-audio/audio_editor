//
//  ProjectSettingsViewPresenter.swift
//

import SwiftUI
import ae_ui_objc

class ProjectSettingsPresenter: ObservableObject {
    var sampleRate: String { "\(presenterObjc.sampleRate) Hz" }
    var channelCount: String { "\(presenterObjc.channelCount)" }

    let timingFractionKinds = TimingFractionKind.allCases
    let timingUnitKinds = TimingUnitKind.allCases

    private let presenterObjc: ProjectSettingsPresenterObjc!

    init(lifetimeId: ProjectLifetimeId) {
        presenterObjc = ProjectSettingsPresenterObjc(projectLifetimeId: lifetimeId)
    }
}
