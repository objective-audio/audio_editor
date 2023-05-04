//
//  ProjectSettingsViewPresenter.swift
//

import SwiftUI

class ProjectSettingsPresenter: ObservableObject {
    var sampleRate: String { "\(self.presenterObjc.sampleRate) Hz" }
    var channelCount: String { "\(self.presenterObjc.channelCount)" }

    let timingFractionKinds = TimingFractionKind.allCases
    let timingUnitKinds = TimingUnitKind.allCases

    private let presenterObjc: ProjectSettingsPresenterObjc

    init(lifetimeId: ProjectLifetimeId) {
        self.presenterObjc = ProjectSettingsPresenterObjc(projectLifetimeId: lifetimeId)
    }
}
