import AppKit
import ae_ui_objc

class AEProjectFormatView: NSView {
    @IBOutlet weak var sampleRateTitleLabel: NSTextField!
    @IBOutlet weak var channelTitleLabel: NSTextField!
    @IBOutlet weak var sampleRatePopUp: NSPopUpButton!
    @IBOutlet weak var channelValueLabel: NSTextField!
    @IBOutlet weak var channelSlider: NSSlider!
    @IBOutlet weak var channelStepper: NSStepper!

    private var presenter: ProjectFormatPresenter!

    static func instantiate() -> AEProjectFormatView? {
        guard let nib = NSNib(nibNamed: "ProjectFormat", bundle: Bundle.module) else {
            return nil
        }

        var objects: NSArray?

        guard nib.instantiate(withOwner: nil, topLevelObjects: &objects) else {
            return nil
        }

        guard let objects = objects else {
            return nil
        }

        return objects.first { object in
            if let object = object as? AEProjectFormatView {
                object.setup()
                return true
            } else {
                return false
            }
        } as? AEProjectFormatView
    }

    func setup() {
        presenter = .init(delegate: self)

        sampleRatePopUp.removeAllItems()

        var titles: [String] = []

        for index in 0..<presenter.availableSampleRatesCount {
            let sampleRate = presenter.availableSampleRate(at: index)
            titles.append("\(sampleRate)Hz")
        }

        sampleRatePopUp.addItems(withTitles: titles)

        let minChannelCount = presenter.minChannelCount
        let maxChannelCount = presenter.maxChannelCount

        channelSlider.minValue = Double(minChannelCount)
        channelSlider.maxValue = Double(maxChannelCount)
        channelStepper.minValue = Double(minChannelCount)
        channelStepper.maxValue = Double(maxChannelCount)

        presenter.setupObserving()
    }

    @IBAction func sampleRatePopUpDidSelect(_ popUp: NSPopUpButton) {
        presenter.setSampleRateIndex(popUp.indexOfSelectedItem)
    }

    @IBAction func channelSliderDidChange(_ sender: NSSlider) {
        presenter.setChannelCount(UInt32(sender.intValue))
    }

    @IBAction func channelStepperDidChange(_ sender: NSStepper) {
        presenter.setChannelCount(UInt32(sender.intValue))
    }
}

extension AEProjectFormatView: ProjectFormatPresenterDelegate {
    func sampleRateIndexDidChange(_ index: Int) {
        sampleRatePopUp.selectItem(at: index)
    }

    func channelCountDidChange(_ count: UInt32) {
        channelValueLabel.stringValue = "\(count)"
        channelStepper.intValue = Int32(count)
        channelSlider.intValue = Int32(count)
    }
}
