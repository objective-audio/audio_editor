import ae_ui_objc

class AEMetalViewController: AEMetalViewControllerObjc {
    override func showModuleNameSheet(with lifetimeId: ProjectSubLifetimeId) {
        let vc = AEModuleNameViewController.instantiate(sheetLifetimeId: lifetimeId)
        presentAsSheet(vc)
    }

    override func showMarkerNameSheet(with lifetimeId: ProjectSubLifetimeId) {
        let vc = AEMarkerNameViewController.instantiate(sheetLifetimeId: lifetimeId)
        presentAsSheet(vc)
    }

    override func hideModal() {
        guard let modals = presentedViewControllers else {
            return
        }

        for modal in modals {
            dismiss(modal)
        }
    }

    override func showSelectFileForImportDialog(with lifetimeId: ProjectSubLifetimeId) {
        let panel = NSOpenPanel()
        panel.allowedContentTypes = [.audio]

        panel.begin { [weak self, weak panel] result in
            guard let self, let panel else { return }
            self.didSelectFileForImportDialog(with: lifetimeId, result: result, url: panel.url)
        }
    }

    override func showSelectFileForExportDialog(with lifetimeId: ProjectSubLifetimeId) {
        let panel = NSSavePanel()
        panel.canCreateDirectories = true
        panel.allowedContentTypes = [.audio]
        panel.nameFieldStringValue = "Untitled"

        panel.begin { [weak self, weak panel] result in
            guard let self, let panel else { return }
            self.didSelectFileForExportDialog(with: lifetimeId, result: result, url: panel.url)
        }
    }
}

extension AEMetalViewController: NSMenuItemValidation {
    override func validateMenuItem(_ menuItem: NSMenuItem) -> Bool {
        super.validateMenuItem(menuItem)
    }
}
