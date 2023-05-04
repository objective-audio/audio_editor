//
//  ProjectSettingsView.swift
//

import SwiftUI

protocol PresenterForProjectSettings: ObservableObject {
    var sampleRate: String { get }
    var channelCount: String { get }
}

extension ProjectSettingsPresenter: PresenterForProjectSettings {}

struct ProjectSettingsView<Presenter: PresenterForProjectSettings>: View {
    @ObservedObject var presenter: Presenter

    init(presenter: Presenter) {
        self.presenter = presenter
    }

    var body: some View {
        Form {
            Section {
                HStack {
                    Text("Sampling Rate")
                    Spacer()
                    Text(presenter.sampleRate)
                }
                HStack {
                    Text("Channel Count")
                    Spacer()
                    Text(presenter.channelCount)
                }
            } header: {
                Text("Project Format")
            }
        }
        .formStyle(.grouped)
        .frame(minWidth: 400,
               minHeight: 140)
    }
}

struct ProjectSettingsView_Previews: PreviewProvider {
    class PreviewPresenter: PresenterForProjectSettings {
        let sampleRate: String = "48000"
        let channelCount: String = "2"
    }

    static var previews: some View {
        ProjectSettingsView<PreviewPresenter>(presenter: PreviewPresenter())
    }
}
