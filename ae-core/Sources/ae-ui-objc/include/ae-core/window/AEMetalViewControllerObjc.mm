//
//  AEMetalViewController.mm
//

#import "AEMetalViewControllerObjc.h"
#import <UniformTypeIdentifiers/UTCoreTypes.h>
#include <ae-core/audio_editor_core_umbrella.h>
#include <ae-core/global/ae_ui_hierarchy.h>
#include <ae-core/global/utils/ae_json_utils.h>
#include <ae-core/global/value_types/ae_project_lifetime_id+objc.h>
#import <ae-core/global/value_types/ae_project_sub_lifetime_id+objc.h>
#include <ae-core/project_editing/features/ae_project_action_sender.h>
#include <ae-core/project_editing/lifecycles/ae_project_modal_lifecycle.h>
#include <ae-core/ui_base/features/event_handling/presenter/ae_event_handling_presenter.h>
#import <ae-core/window/AEMetalViewObjc.h>
#include <cpp-utils/assertion.h>
#include <cpp-utils/cf_utils.h>
#include <cpp-utils/unowned.h>
#include <ae-core/app/lifecycles/ae_ui_resource_lifecycle.hpp>
#include <ae-core/module_name_sheet/features/ae_module_name_editor.hpp>
#include <ae-core/ui_base/ae_ui_base_lifetime.hpp>
#include <ae-core/ui_resource/ae_ui_resource_lifetime.hpp>

using namespace yas;
using namespace yas::ae;

@interface AEMetalViewControllerObjc ()

@end

@implementation AEMetalViewControllerObjc {
    project_lifetime_id _project_lifetime_id;
    std::weak_ptr<ui_resource_lifetime> _resource_lifetime;
    std::shared_ptr<event_handling_presenter> _event_handling_presenter;
    std::weak_ptr<project_modal_lifecycle> _project_modal_lifecycle;
    std::weak_ptr<project_action_sender> _action_sender;
    observing::canceller_pool _pool;
    observing::cancellable_ptr _sheet_canceller;
}

- (AEMetalViewObjc *)aeMetalView {
    if ([self.view isKindOfClass:[AEMetalViewObjc class]]) {
        return static_cast<AEMetalViewObjc *>(self.view);
    } else {
        throw std::runtime_error("self.view is not AEMetalView.");
    }
}

- (void)viewDidDisappear {
    [super viewDidDisappear];

    hierarchy::app_lifetime()->ui_resource_lifecycle->remove_lifetime_for_project_lifetime_id(
        self->_project_lifetime_id);
}

- (void)setupWithProjectLifetimeId:(ProjectLifetimeId *)lifetimeId {
    [self setupWithProjectLifetimeID:[lifetimeId raw_value]];
}

- (void)setupWithProjectLifetimeID:(project_lifetime_id const &)lifetime_id {
    auto const &ui_resource_lifecycle = hierarchy::app_lifetime()->ui_resource_lifecycle;
    auto const &project_editing_lifetime = hierarchy::project_editing_lifetime_for_id(lifetime_id);

    [self setupWithProjectLifetimeID:lifetime_id
                 uiResourceLifecycle:ui_resource_lifecycle
                    actionController:project_editing_lifetime->action_sender
               projectModalLifecycle:project_editing_lifetime->modal_lifecycle];
}

- (void)setupWithProjectLifetimeID:(project_lifetime_id const &)project_lifetime_id
               uiResourceLifecycle:(std::shared_ptr<ae::ui_resource_lifecycle> const &)ui_resource_lifecycle
                  actionController:(std::shared_ptr<project_action_sender> const &)action_sender
             projectModalLifecycle:(std::shared_ptr<project_modal_lifecycle> const &)project_modal_lifecycle {
    self->_project_lifetime_id = project_lifetime_id;

    auto const metal_system = ui::metal_system::make_shared(
        objc_ptr_with_move_object(MTLCreateSystemDefaultDevice()).object(), self.metalView);
    auto const standard = ui::standard::make_shared([self view_look], metal_system);

    ui_resource_lifecycle->add_lifetime(standard, self->_project_lifetime_id);
    self->_resource_lifetime = ui_resource_lifecycle->lifetime_for_project_lifetime_id(self->_project_lifetime_id);
    self->_event_handling_presenter = event_handling_presenter::make_shared(self->_project_lifetime_id);

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

                case kind::module_name_sheet: {
                    auto const &lifetime = get<module_name_sheet_lifetime>(sub_lifetime);
                    auto const lifetimeId = [[ProjectSubLifetimeId alloc] initWithRawValue:lifetime->lifetime_id];
                    [self showModuleNameSheetWithLifetimeId:lifetimeId];
                } break;

                case kind::marker_name_sheet: {
                    auto const &lifetime = get<marker_name_sheet_lifetime>(sub_lifetime);
                    auto const lifetimeId = [[ProjectSubLifetimeId alloc] initWithRawValue:lifetime->lifetime_id];
                    [self showMarkerNameSheetWithLifetimeId:lifetimeId];
                } break;

                case kind::file_import_dialog: {
                    auto const &lifetime = get<file_import_dialog_lifetime>(sub_lifetime);
                    auto const lifetimeId = [[ProjectSubLifetimeId alloc] initWithRawValue:lifetime->lifetime_id];
                    [self showSelectFileForImportDialogWithLifetimeId:lifetimeId];
                } break;

                case kind::file_export_dialog: {
                    auto const &lifetime = get<file_export_dialog_lifetime>(sub_lifetime);
                    auto const lifetimeId = [[ProjectSubLifetimeId alloc] initWithRawValue:lifetime->lifetime_id];
                    [self showSelectFileForExportDialogWithLifetimeId:lifetimeId];
                } break;

                case kind::dialog: {
                    auto const &lifetime = get<dialog_lifetime>(sub_lifetime);
                    switch (lifetime->content) {
                        case dialog_content::select_file_for_import: {
                            auto const lifetimeId =
                                [[ProjectSubLifetimeId alloc] initWithRawValue:lifetime->lifetime_id];
                            [self showSelectFileForImportDialogWithLifetimeId:lifetimeId];
                        } break;
                        case dialog_content::select_file_for_export: {
                            auto const lifetimeId =
                                [[ProjectSubLifetimeId alloc] initWithRawValue:lifetime->lifetime_id];
                            [self showSelectFileForExportDialogWithLifetimeId:lifetimeId];
                        } break;
                    }
                } break;

                case kind::context_menu:
                case kind::time_editing:
                    break;
            }
        })
        .end()
        ->add_to(self->_pool);

    [self.aeMetalView setupWithProjectLifetimeId:[[ProjectLifetimeId alloc] initWithRawValue:project_lifetime_id]];
}

- (IBAction)jumpPrevious:(NSMenuItem *)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(editing_action_name::jump_previous);
    }
}

- (IBAction)jumpNext:(NSMenuItem *)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(editing_action_name::jump_next);
    }
}

- (IBAction)jumpToBeginning:(NSMenuItem *)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(editing_action_name::jump_to_beginning);
    }
}

- (IBAction)jumpToEnd:(NSMenuItem *)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(editing_action_name::jump_to_end);
    }
}

- (IBAction)jumpToTrackBeginning:(NSMenuItem *)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(editing_action_name::jump_to_track_beginning);
    }
}

- (IBAction)jumpToTrackEnd:(NSMenuItem *)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(editing_action_name::jump_to_track_end);
    }
}

- (IBAction)insertMarker:(NSMenuItem *)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(editing_action_name::insert_marker);
    }
}

- (IBAction)returnToZero:(NSMenuItem *)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(editing_action_name::return_to_zero);
    }
}

- (IBAction)undo:(NSMenuItem *)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(editing_action_name::undo);
    }
}

- (IBAction)redo:(NSMenuItem *)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(editing_action_name::redo);
    }
}

- (IBAction)importFromFile:(id)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(editing_action_name::select_file_for_import);
    }
}

- (IBAction)exportToFile:(id)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(editing_action_name::select_file_for_export);
    }
}

- (IBAction)cut:(id)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(editing_action_name::cut);
    }
}

- (IBAction)copy:(id)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(editing_action_name::copy);
    }
}

- (IBAction)paste:(id)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(editing_action_name::paste);
    }
}

- (IBAction)purge:(id)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(editing_action_name::purge);
    }
}

- (IBAction)openProjectSettings:(id)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(editing_action_name::open_project_settings);
    }
}

- (IBAction)selectAll:(id)sender {
    if (auto const action_sender = self->_action_sender.lock()) {
        action_sender->send(editing_action_name::select_all_modules);
    }
}

#pragma mark -

- (BOOL)validateMenuItem:(NSMenuItem *)menuItem {
    if (auto const action_name = [self actionNameForSelector:menuItem.action]) {
        return self->_event_handling_presenter->responds_to_action({action_name.value(), ""});
    }
    return NO;
}

- (std::optional<action_name>)actionNameForSelector:(SEL)selector {
    if (selector == @selector(jumpPrevious:)) {
        return editing_action_name::jump_previous;
    } else if (selector == @selector(jumpNext:)) {
        return editing_action_name::jump_next;
    } else if (selector == @selector(jumpToBeginning:)) {
        return editing_action_name::jump_to_beginning;
    } else if (selector == @selector(jumpToEnd:)) {
        return editing_action_name::jump_to_end;
    } else if (selector == @selector(jumpToTrackBeginning:)) {
        return editing_action_name::jump_to_track_beginning;
    } else if (selector == @selector(jumpToTrackEnd:)) {
        return editing_action_name::jump_to_track_end;
    } else if (selector == @selector(insertMarker:)) {
        return editing_action_name::insert_marker;
    } else if (selector == @selector(returnToZero:)) {
        return editing_action_name::return_to_zero;
    } else if (selector == @selector(undo:)) {
        return editing_action_name::undo;
    } else if (selector == @selector(redo:)) {
        return editing_action_name::redo;
    } else if (selector == @selector(importFromFile:)) {
        return editing_action_name::select_file_for_import;
    } else if (selector == @selector(exportToFile:)) {
        return editing_action_name::select_file_for_export;
    } else if (selector == @selector(cut:)) {
        return editing_action_name::cut;
    } else if (selector == @selector(copy:)) {
        return editing_action_name::copy;
    } else if (selector == @selector(paste:)) {
        return editing_action_name::paste;
    } else if (selector == @selector(purge:)) {
        return editing_action_name::purge;
    } else if (selector == @selector(openProjectSettings:)) {
        return editing_action_name::open_project_settings;
    } else if (selector == @selector(selectAll:)) {
        return editing_action_name::select_all_modules;
    }

    return std::nullopt;
}

#pragma mark -

- (void)showSelectFileForImportDialogWithLifetimeId:(ProjectSubLifetimeId *)lifetimeId {
    NSAssert(NO, @"should be overridden");
}

- (void)didSelectFileForImportDialogWithLifetimeId:(ProjectSubLifetimeId *)lifetimeId
                                            result:(NSModalResponse)result
                                               url:(NSURL *)url {
    if (auto const lifecycle = self->_project_modal_lifecycle.lock()) {
        lifecycle->remove_file_import_dialog([lifetimeId raw_value]);
    } else {
        assertion_failure_if_not_test();
    }

    if (result == NSModalResponseOK && url != nil) {
        auto const path = to_string((__bridge CFStringRef)url.path);
        if (auto const action_sender = self->_action_sender.lock()) {
            action_sender->send(editing_action_name::import_from_file, path);
        }
    }
}

- (void)showSelectFileForExportDialogWithLifetimeId:(ProjectSubLifetimeId *)lifetimeId {
    NSAssert(NO, @"should be overridden");
}

- (void)didSelectFileForExportDialogWithLifetimeId:(ProjectSubLifetimeId *)lifetimeId
                                            result:(NSModalResponse)result
                                               url:(NSURL *)url {
    if (auto const lifecycle = self->_project_modal_lifecycle.lock()) {
        lifecycle->remove_file_export_dialog([lifetimeId raw_value]);
    } else {
        assertion_failure_if_not_test();
    }

    if (result == NSModalResponseOK && url != nil) {
        auto const path = to_string((__bridge CFStringRef)url.path);
        if (auto const action_sender = self->_action_sender.lock()) {
            action_sender->send(editing_action_name::export_to_file, path);
        }
    }
}

- (void)showModuleNameSheetWithLifetimeId:(ProjectSubLifetimeId *)lifetimeId {
    NSAssert(NO, @"should be overridden");
}

- (void)showMarkerNameSheetWithLifetimeId:(ProjectSubLifetimeId *)lifetimeId {
    NSAssert(NO, @"should be overridden");
}

- (void)hideModal {
    NSAssert(NO, @"should be overridden");
}

@end
