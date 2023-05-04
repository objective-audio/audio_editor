//
//  AppSettingsView.swift
//

import SwiftUI

protocol PresenterForAppSettings: ObservableObject {
    var timingFractionKinds: [TimingFractionKind] { get }
    var timingUnitKinds: [TimingUnitKind] { get }
    var selectedFraction: TimingFractionKind { get set }
    var selectedUnit: TimingUnitKind { get set }
}

extension AppSettingsPresenter: PresenterForAppSettings {}

struct AppSettingsView<Presenter: PresenterForAppSettings>: View {
    @ObservedObject var presenter: Presenter

    init(presenter: Presenter) {
        self.presenter = presenter
    }

    var body: some View {
        Form {
            Section {
                Picker("Timing Unit", selection: $presenter.selectedUnit) {
                    ForEach(presenter.timingUnitKinds) { kind in
                        Text("\(kind.text)")
                    }
                }
                Picker("Timing Fraction", selection: $presenter.selectedFraction) {
                    ForEach(presenter.timingFractionKinds) { kind in
                        Text("\(kind.text)")
                    }
                }
            } header: {
                Text("Nudge / Grid")
            }
        }
        .formStyle(.grouped)
        .frame(minWidth: 400,
               minHeight: 140)
    }
}

struct AppSettingsView_Previews: PreviewProvider {
    class PreviewPresenter: PresenterForAppSettings {
        let timingFractionKinds = TimingFractionKind.allCases
        let timingUnitKinds = TimingUnitKind.allCases
        var selectedFraction: TimingFractionKind = .sample
        var selectedUnit: TimingUnitKind = .fraction
    }

    static var previews: some View {
        AppSettingsView<PreviewPresenter>(presenter: PreviewPresenter())
    }
}

private extension TimingFractionKind {
    var text: String {
        switch self {
        case .sample:
            return "sample"
        case .milisecond:
            return "milisecond"
        case .frame30:
            return "frame30"
        }
    }
}

private extension TimingUnitKind {
    var text: String {
        switch self {
        case .fraction:
            return "fraction"
        case .seconds:
            return "seconds"
        case .minutes:
            return "minutes"
        case .hours:
            return "hours"
        }
    }
}
