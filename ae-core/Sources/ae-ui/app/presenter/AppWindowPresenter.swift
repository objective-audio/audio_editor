import AppKit
import ae_ui_objc

public class AppWindowPresenter: AppWindowPresenterObjc {
    private var projectControllers: Set<ProjectWindowController> = .init()
    private var projectSettingsControllers: Set<ProjectSettingsWindowController> = .init()
    private var appSettingsControllers: Set<AppSettingsWindowController> = .init()

    public override func openProjectSetupDialog(
        completion: @escaping (NSApplication.ModalResponse, URL) -> Void
    ) {
        let panel = NSOpenPanel()
        panel.allowsMultipleSelection = false
        panel.canChooseFiles = false
        panel.canChooseDirectories = true
        panel.canCreateDirectories = true
        panel.accessoryView = AEProjectFormatView.instantiate()

        panel.begin { [weak panel] result in
            guard let panel, let url = panel.url else { return }
            completion(result, url)
        }
    }

    public override func makeAndShowProjectSettings(with lifetimeId: ProjectLifetimeId) {
        let controller = ProjectSettingsWindowController.instantiate(lifetimeId: lifetimeId)
        projectSettingsControllers.insert(controller)
        controller.showWindow(nil)
    }

    public override func disposeProjectSettings(with lifetimeId: ProjectLifetimeId) {
        var copiedControllers = projectSettingsControllers

        for controller in projectSettingsControllers where controller.lifetimeId == lifetimeId {
            controller.close()
            copiedControllers.remove(controller)
        }

        projectSettingsControllers = copiedControllers
    }

    public override func makeAndShowAppSettings(with lifetimeId: AppSettingsLifetimeId) {
        let controller = AppSettingsWindowController.instantiate(lifetimeId: lifetimeId)
        appSettingsControllers.insert(controller)
        controller.showWindow(nil)
    }

    public override func disposeAppSettings(with lifetimeId: AppSettingsLifetimeId) {
        var copiedControllers = appSettingsControllers

        for controller in appSettingsControllers where controller.lifetimeId == lifetimeId {
            controller.close()
            copiedControllers.remove(controller)
        }

        appSettingsControllers = copiedControllers
    }

    public override func makeAndShowProject(with lifetimeId: ProjectLifetimeId) {
        let controller = ProjectWindowController.instantiate(lifetimeId: lifetimeId)
        projectControllers.insert(controller)
        controller.showWindow(nil)
    }

    public override func showProject(with lifetimeId: ProjectLifetimeId) {
        for controller in projectControllers where controller.lifetimeId == lifetimeId {
            controller.showWindow(nil)
            return
        }
    }

    public override func disposeProject(with lifetimeId: ProjectLifetimeId) {
        var copiedControllers = projectControllers

        for controller in projectControllers where controller.lifetimeId == lifetimeId {
            controller.close()
            copiedControllers.remove(controller)
        }

        projectControllers = copiedControllers
    }
}
