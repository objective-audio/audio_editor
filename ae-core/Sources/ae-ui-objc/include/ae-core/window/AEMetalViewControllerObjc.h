//
//  AEMetalViewController.h
//

#import <ae-core/global/value_types/ProjectLifetimeId.h>
#import <ui/metal/view/yas_ui_metal_view_controller_objc.h>

NS_ASSUME_NONNULL_BEGIN

@class ProjectSubLifetimeId;

@interface AEMetalViewControllerObjc : YASUIMetalViewController

- (void)setupWithProjectLifetimeId:(ProjectLifetimeId *)lifetimeId;

- (void)showModuleNameSheetWithLifetimeId:(ProjectSubLifetimeId *)lifetimeId;
- (void)showMarkerNameSheetWithLifetimeId:(ProjectSubLifetimeId *)lifetimeId;

- (void)showSelectFileForImportDialogWithLifetimeId:(ProjectSubLifetimeId *)lifetimeId;
- (void)didSelectFileForImportDialogWithLifetimeId:(ProjectSubLifetimeId *)lifetimeId
                                            result:(NSModalResponse)result
                                               url:(nullable NSURL *)url;
- (void)showSelectFileForExportDialogWithLifetimeId:(ProjectSubLifetimeId *)lifetimeId;
- (void)didSelectFileForExportDialogWithLifetimeId:(ProjectSubLifetimeId *)lifetimeId
                                            result:(NSModalResponse)result
                                               url:(nullable NSURL *)url;

- (void)hideModal;

- (BOOL)validateMenuItem:(NSMenuItem *)menuItem NS_SWIFT_NAME(validateMenuItem(_:));

@end

NS_ASSUME_NONNULL_END
