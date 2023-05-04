//
//  AppSettingsPresenter.swift
//

import Foundation

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

class AppSettingsPresenter: ObservableObject {
    let timingFractionKinds = TimingFractionKind.allCases
    let timingUnitKinds = TimingUnitKind.allCases

    private let presenterObjc: AppSettingsPresenterObjc

    var selectedFraction: TimingFractionKind {
        get { self.presenterObjc.timingFractionKind }
        set { self.presenterObjc.timingFractionKind = newValue }
    }

    var selectedUnit: TimingUnitKind {
        get { self.presenterObjc.timingUnitKind }
        set { self.presenterObjc.timingUnitKind = newValue }
    }

    init() {
        let presenter = AppSettingsPresenterObjc()

        self.presenterObjc = presenter

        presenter.observe { [weak self] in
            self?.objectWillChange.send()
        }
    }
}
