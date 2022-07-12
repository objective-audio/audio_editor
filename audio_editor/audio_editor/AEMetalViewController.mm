//
//  AEMetalViewController.mm
//

#import "AEMetalViewController.h"
#import <UniformTypeIdentifiers/UTCoreTypes.h>
#include <audio_editor_core/ae_json_utils.h>
#include <audio_editor_core/ae_project_action_sender.h>
#include <audio_editor_core/ae_project_modal_lifecycle.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_root.h>
#include <audio_editor_core/ae_ui_root_lifecycle.h>
#include <audio_editor_core/ae_ui_root_lifetime.h>
#include <audio_editor_core/audio_editor_core_umbrella.h>
#include <cpp_utils/yas_assertion.h>
#include <cpp_utils/yas_cf_utils.h>
#include <cpp_utils/yas_unowned.h>
#import "AEMetalView.h"
#import "AEModuleNameViewController.h"

using namespace yas;
using namespace yas::ae;

@interface AEMetalViewController () <NSMenuItemValidation>

@end

@implementation AEMetalViewController {
    window_lifetime_id _window_lifetime_id;
    std::weak_ptr<ui_root_lifetime> _root_lifetime;
    std::weak_ptr<project_modal_lifecycle> _project_modal_lifecycle;
    std::weak_ptr<project_action_sender> _action_sender;
    observing::canceller_pool _pool;
    observing::cancellable_ptr _sheet_canceller;
}

- (AEMetalView *)aeMetalView {
    if ([self.view isKindOfClass:[AEMetalView class]]) {
        return static_cast<AEMetalView *>(self.view);
    } else {
        throw std::runtime_error("self.view is not AEMetalView.");
    }
}

- (void)viewDidDisappear {
    [super viewDidDisappear];

    hierarchy::app_lifetime()->ui_root_lifecycle->remove_lifetime_for_window_lifetime_id(self->_window_lifetime_id);
}

- (void)setupWithWindowLifetimeID:(window_lifetime_id const &)lifetime_id {
    auto const &ui_root_lifecycle = hierarchy::app_lifetime()->ui_root_lifecycle;
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(lifetime_id);

    [self setupWithWindowLifetimeID:lifetime_id
                    uiRootLifecycle:ui_root_lifecycle
                   actionController:project_lifetime->action_sender
              projectModalLifecycle:project_lifetime->modal_lifecycle];
}

- (void)setupWithWindowLifetimeID:(window_lifetime_id const &)window_lifetime_id
                  uiRootLifecycle:(std::shared_ptr<ae::ui_root_lifecycle> const &)ui_root_lifecycle
                 actionController:(std::shared_ptr<project_action_sender> const &)action_sender
            projectModalLifecycle:(std::shared_ptr<project_modal_lifecycle> const &)project_modal_lifecycle {
    self->_window_lifetime_id = window_lifetime_id;

    auto const metal_system = ui::metal_system::make_shared(
        objc_ptr_with_move_object(MTLCreateSystemDefaultDevice()).object(), self.metalView);
    auto const standard = ui::standard::make_shared([self view_look], metal_system);

    ui_root_lifecycle->add_lifetime(standard, self->_window_lifetime_id);
    self->_root_lifetime = ui_root_lifecycle->lifetime_for_window_lifetime_id(self->_window_lifetime_id);

    self->_action_sender = action_sender;
    self->_project_modal_lifecycle = project_modal_lifecycle;

    [self configure_with_metal_system:metal_system
                             renderer:standard->renderer()
                        event_manager:standard->event_manager()];

    auto *const unowned_self = make_unowned(self);

    project_modal_lifecycle
        ->observe([unowned_self](std::optional<project_modal_sub_lifetime> const &sub_lifetime) {
            auto *const self = unowned_self.object;

            using kind = project_modal_sub_lifetime_kind;

            switch (to_kind(sub_lifetime)) {
                case kind::none:
                    [self hideModal];
                    break;

                case kind::sheet: {
                    auto const &lifetime = get<sheet_lifetime>(sub_lifetime);
                    switch (lifetime->content.kind) {
                        case sheet_kind::module_name:
                            [self showModuleNameSheetWithLifetimeId:lifetime->lifetime_id
                                                              value:lifetime->content.value];
                            break;
                    }
                } break;

                case kind::dialog: {
                    auto const &lifetime = get<dialog_lifetime>(sub_lifetime);
                    switch (lifetime->content) {
                        case dialog_content::select_file_for_import:
                            [self showSelectFileForImportDialogWithLifetimeId:lifetime->lifetime_id];
                            break;
                        case dialog_content::select_file_for_export:
                            [self showSelectFileForExportDialogWithLifetimeId:lifetime->lifetime_id];
                            break;
                    }
                } break;

                case kind::context_menu:
                case kind::time_editor:
                    break;
            }
        })
        .end()
        ->add_to(self->_pool);

    [self.aeMetalView setupWithWindowLifetimeID:window_lifetime_id];
}

- (IBAction)jumpPrevious:(NSMenuItem *)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(action_kind::jump_previous);
    }
}

- (IBAction)jumpNext:(NSMenuItem *)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(action_kind::jump_next);
    }
}

- (IBAction)jumpToBeginning:(NSMenuItem *)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(action_kind::jump_to_beginning);
    }
}

- (IBAction)jumpToEnd:(NSMenuItem *)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(action_kind::jump_to_end);
    }
}

- (IBAction)insertMarker:(NSMenuItem *)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(action_kind::insert_marker);
    }
}

- (IBAction)returnToZero:(NSMenuItem *)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(action_kind::return_to_zero);
    }
}

- (IBAction)undo:(NSMenuItem *)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(action_kind::undo);
    }
}

- (IBAction)redo:(NSMenuItem *)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(action_kind::redo);
    }
}

- (IBAction)importFromFile:(id)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(action_kind::select_file_for_import);
    }
}

- (IBAction)exportToFile:(id)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(action_kind::select_file_for_export);
    }
}

- (IBAction)cut:(id)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(action_kind::cut);
    }
}

- (IBAction)copy:(id)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(action_kind::copy);
    }
}

- (IBAction)paste:(id)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(action_kind::paste);
    }
}

- (IBAction)purge:(id)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(action_kind::purge);
    }
}

#pragma mark -

- (BOOL)validateMenuItem:(NSMenuItem *)menuItem {
    if (auto const action_kind = [self actionKindForSelector:menuItem.action]) {
        if (auto const ui_root_lifetime = self->_root_lifetime.lock()) {
            return ui_root_lifetime->root->responds_to_action({action_kind.value(), ""});
        }
    }
    return NO;
}

- (std::optional<action_kind>)actionKindForSelector:(SEL)selector {
    if (selector == @selector(jumpPrevious:)) {
        return action_kind::jump_previous;
    } else if (selector == @selector(jumpNext:)) {
        return action_kind::jump_next;
    } else if (selector == @selector(jumpToBeginning:)) {
        return action_kind::jump_to_beginning;
    } else if (selector == @selector(jumpToEnd:)) {
        return action_kind::jump_to_end;
    } else if (selector == @selector(insertMarker:)) {
        return action_kind::insert_marker;
    } else if (selector == @selector(returnToZero:)) {
        return action_kind::return_to_zero;
    } else if (selector == @selector(undo:)) {
        return action_kind::undo;
    } else if (selector == @selector(redo:)) {
        return action_kind::redo;
    } else if (selector == @selector(importFromFile:)) {
        return action_kind::select_file_for_import;
    } else if (selector == @selector(exportToFile:)) {
        return action_kind::select_file_for_export;
    } else if (selector == @selector(cut:)) {
        return action_kind::cut;
    } else if (selector == @selector(copy:)) {
        return action_kind::copy;
    } else if (selector == @selector(paste:)) {
        return action_kind::paste;
    } else if (selector == @selector(purge:)) {
        return action_kind::purge;
    }

    return std::nullopt;
}

#pragma mark -

- (void)showSelectFileForImportDialogWithLifetimeId:(dialog_lifetime_id const &)lifetime_id {
    auto *const panel = [NSOpenPanel openPanel];
    panel.allowedContentTypes = @[UTTypeAudio];

    auto *const unowned_self = make_unowned(self);
    auto *const unowned_panel = make_unowned(panel);

    [panel beginWithCompletionHandler:[unowned_self, unowned_panel, lifetime_id](NSModalResponse result) {
        auto *const self = unowned_self.object;
        auto *const panel = unowned_panel.object;

        if (auto const lifecycle = self->_project_modal_lifecycle.lock()) {
            lifecycle->remove_dialog(lifetime_id);
        } else {
            assertion_failure_if_not_test();
        }

        if (result == NSModalResponseOK) {
            auto const path = to_string((__bridge CFStringRef)panel.URL.path);
            if (auto const action_sender = self->_action_sender.lock()) {
                action_sender->send(action_kind::import_from_file, path);
            }
        }
    }];
}

- (void)showSelectFileForExportDialogWithLifetimeId:(dialog_lifetime_id const &)lifetime_id {
    auto *const panel = [NSSavePanel savePanel];
    panel.canCreateDirectories = YES;
    panel.allowedContentTypes = @[UTTypeAudio];
    panel.nameFieldStringValue = @"Untitled";

    auto *const unowned_self = make_unowned(self);
    auto *const unowned_panel = make_unowned(panel);

    [panel beginWithCompletionHandler:[unowned_self, unowned_panel, lifetime_id](NSModalResponse result) {
        auto *const self = unowned_self.object;
        auto *const panel = unowned_panel.object;

        if (auto const lifecycle = self->_project_modal_lifecycle.lock()) {
            lifecycle->remove_dialog(lifetime_id);
        } else {
            assertion_failure_if_not_test();
        }

        if (result == NSModalResponseOK) {
            auto const path = to_string((__bridge CFStringRef)panel.URL.path);
            if (auto const action_sender = self->_action_sender.lock()) {
                action_sender->send(action_kind::export_to_file, path);
            }
        }
    }];
}

- (void)showModuleNameSheetWithLifetimeId:(sheet_lifetime_id const &)lifetime_id value:(std::string const &)value {
    auto const range = to_time_range(value);
    if (!range.has_value()) {
        assertion_failure_if_not_test();
        return;
    }

    auto *const vc = [AEModuleNameViewController instantiateWithSheetLifetimeId:lifetime_id moduleRange:range.value()];

    [self presentViewControllerAsSheet:vc];
}

- (void)hideModal {
    for (NSViewController *modal in self.presentedViewControllers) {
        [self dismissViewController:modal];
    }
}

@end
