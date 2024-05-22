import Cocoa
import ae_ui
import ae_ui_objc

@main
class AppDelegate: NSObject, NSApplicationDelegate {
    private let presenter: AppDelegatePresenter = .init()
    private var windowPresenter: AppWindowPresenter?

    func applicationDidFinishLaunching(_ aNotification: Notification) {
        presenter.didFinishLaunching()
        windowPresenter = .init()
    }

    func applicationWillTerminate(_ aNotification: Notification) {
        // Insert code here to tear down your application
    }

    func applicationSupportsSecureRestorableState(_ app: NSApplication) -> Bool {
        return true
    }

    override func responds(to aSelector: Selector!) -> Bool {
        let action = presenter.action(for: aSelector)
        if windowPresenter?.responds(to: action) ?? false {
            return true
        } else {
            return super.responds(to: aSelector)
        }
    }

    @IBAction func openDocument(_ sender: Any) {
        windowPresenter?.perform(.openProject)
    }

    @IBAction func openAppSettings(_ sender: Any) {
        windowPresenter?.perform(.openAppSettings)
    }
}
