import AppKit
import Foundation

@objcMembers
public class AESwiftBundle: NSObject {
    public static func color(name: String) throws -> NSColor {
        enum GetColorError: Error {
            case notFound
        }

        var nsColor: NSColor?

        NSApplication.shared.effectiveAppearance.performAsCurrentDrawingAppearance {
            nsColor = NSColor(named: name, bundle: Bundle.module)?.usingColorSpace(.deviceRGB)
        }

        guard let nsColor else {
            throw GetColorError.notFound
        }

        return nsColor
    }
}
