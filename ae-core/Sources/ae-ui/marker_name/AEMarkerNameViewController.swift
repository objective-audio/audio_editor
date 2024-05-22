import ae_ui_objc

class AEMarkerNameViewController: NSViewController {
    @IBOutlet weak var titleLabel: NSTextField!
    @IBOutlet weak var nameTextField: NSTextField!

    private var presenter: MarkerNamePresenterObjc!

    static func instantiate(sheetLifetimeId: ProjectSubLifetimeId) -> AEMarkerNameViewController {
        let storyboard = NSStoryboard(name: "MarkerName", bundle: Bundle.module)
        guard let vc = storyboard.instantiateInitialController() as? AEMarkerNameViewController
        else {
            fatalError()
        }
        vc.setup(withSheetLifetimeId: sheetLifetimeId)
        return vc
    }

    func setup(withSheetLifetimeId lifetimeId: ProjectSubLifetimeId) {
        presenter = .init()
        presenter.setup(withSheetLifetimeId: lifetimeId)
        self.preferredContentSize = .init(width: 400.0, height: 150.0)
    }

    override func viewDidLoad() {
        super.viewDidLoad()

        nameTextField.stringValue = presenter.name
    }

    @IBAction func done(_ sender: NSButton) {
        presenter.done(withName: nameTextField.stringValue)
    }

    @IBAction func cancel(_ sender: NSButton) {
        presenter.cancel()
    }
}
