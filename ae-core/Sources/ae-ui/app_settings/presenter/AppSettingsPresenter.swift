//
//  AppSettingsPresenter.swift
//

import Foundation
import ae_ui_objc

extension TimingFractionKind: Identifiable, CaseIterable {
    public static let allCases: [TimingFractionKind] = [
        .sample, .milisecond, .frame30,
    ]

    public var id: Self { self }
}

extension TimingUnitKind: Identifiable, CaseIterable {
    public static let allCases: [TimingUnitKind] = [
        .fraction, .seconds, .minutes, .hours,
    ]

    public var id: Self { self }
}

class AppSettingsPresenter: AppSettingsPresenterObjc, ObservableObject {
    let timingFractionKinds = TimingFractionKind.allCases
    let timingUnitKinds = TimingUnitKind.allCases

    var selectedFraction: TimingFractionKind {
        get { timingFractionKind }
        set { timingFractionKind = newValue }
    }

    var selectedUnit: TimingUnitKind {
        get { timingUnitKind }
        set { timingUnitKind = newValue }
    }

    override init() {
        super.init()

        observe { [weak self] in
            self?.objectWillChange.send()
        }
    }
}
