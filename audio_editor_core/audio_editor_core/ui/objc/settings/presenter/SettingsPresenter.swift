//
//  SettingsViewPresenter.swift
//

import SwiftUI

extension TimingFractionKind: Identifiable, CaseIterable {
    public static let allCases: [TimingFractionKind] = [
        .sample, .milisecond, .frame30
    ]

    public var id: Self { self }
}

extension TimingUnitKind: Identifiable, CaseIterable {
    public static let allCases: [TimingUnitKind] = [
        .fraction, .seconds, .minutes, .hours
    ]

    public var id: Self { self }
}

class SettingsPresenter: ObservableObject {
    var sampleRate: String { "\(self.presenterObjc.sampleRate) Hz" }
    var channelCount: String { "\(self.presenterObjc.channelCount)" }

    let timingFractionKinds = TimingFractionKind.allCases
    let timingUnitKinds = TimingUnitKind.allCases

    private let presenterObjc: SettingsPresenterObjc

    var selectedFraction: TimingFractionKind {
        get { self.presenterObjc.timingFractionKind }
        set { self.presenterObjc.timingFractionKind = newValue }
    }

    var selectedUnit: TimingUnitKind {
        get { self.presenterObjc.timingUnitKind }
        set { self.presenterObjc.timingUnitKind = newValue }
    }

    init(lifetimeId: WindowLifetimeId) {
        let presenter = SettingsPresenterObjc(windowLifetimeId: lifetimeId)

        self.presenterObjc = presenter

        presenter.observe { [weak self] in
            self?.objectWillChange.send()
        }
    }
}
