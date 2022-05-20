//
//  AEMetalViewController.mm
//

#import "AEMetalViewController.h"
#import <UniformTypeIdentifiers/UTCoreTypes.h>
#include <audio_editor_core/ae_action_controller.h>
#include <audio_editor_core/ae_dialog_presenter.h>
#include <audio_editor_core/ae_sheet_presenter.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_root_level.h>
#include <audio_editor_core/ae_ui_root_level_router.h>
#include <audio_editor_core/audio_editor_core_umbrella.h>
#include <cpp_utils/yas_cf_utils.h>
#include <objc_utils/yas_objc_unowned.h>
#import "AEMetalView.h"
#import "AEModuleNameViewController.h"

using namespace yas;
using namespace yas::ae;

@interface AEMetalViewController () <NSMenuItemValidation>

@end

@implementation AEMetalViewController {
    project_id _project_id;
    std::weak_ptr<ui_root_level> _root_level;
    std::shared_ptr<action_controller> _action_controller;
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

- (std::uintptr_t)project_view_id {
    return reinterpret_cast<std::uintptr_t>(self);
}

- (void)viewDidDisappear {
    [super viewDidDisappear];

    hierarchy::app_level()->ui_root_level_router->remove_level_for_view_id(self.project_view_id);
}

- (void)setupWithProjectID:(project_id const &)project_id {
    self->_project_id = project_id;

    auto const metal_system = ui::metal_system::make_shared(
        objc_ptr_with_move_object(MTLCreateSystemDefaultDevice()).object(), self.metalView);
    auto const standard = ui::standard::make_shared([self view_look], metal_system);

    auto const &ui_root_level_router = hierarchy::app_level()->ui_root_level_router;
    ui_root_level_router->add_level(standard, {.project_id = project_id, .view_id = self.project_view_id});
    self->_root_level = ui_root_level_router->level_for_view_id(self.project_view_id);

    auto const &project_level = hierarchy::project_level_for_id(project_id);
    self->_action_controller = action_controller::make_shared(project_id);

    [self configure_with_metal_system:metal_system
                             renderer:standard->renderer()
                        event_manager:standard->event_manager()];

    auto *const unowned_self = [[YASUnownedObject<AEMetalViewController *> alloc] initWithObject:self];

    project_level->dialog_presenter
        ->observe_event([unowned_self](dialog_event const &event) {
            auto *const self = unowned_self.object;

            switch (event) {
                case dialog_event::select_file_for_export: {
                    [self showSelectFileForExportDialog];
                } break;
            }
        })
        .end()
        ->add_to(self->_pool);

    project_level->sheet_presenter
        ->observe_event([unowned_self](sheet_event const &event) {
            auto *const self = unowned_self.object;

            switch (event.kind) {
                case sheet_kind_escape::module_name:
                    [self showModuleNameSheetWithValue:event.value];
                    break;
            }
        })
        .end()
        ->add_to(self->_pool);

    [self.aeMetalView setupWithProjectID:project_id];
}

- (IBAction)jumpPrevious:(NSMenuItem *)sender {
    self->_action_controller->handle_action(action_kind::jump_previous);
}

- (IBAction)jumpNext:(NSMenuItem *)sender {
    self->_action_controller->handle_action(action_kind::jump_next);
}

- (IBAction)jumpToBeginning:(NSMenuItem *)sender {
    self->_action_controller->handle_action(action_kind::jump_to_beginning);
}

- (IBAction)jumpToEnd:(NSMenuItem *)sender {
    self->_action_controller->handle_action(action_kind::jump_to_end);
}

- (IBAction)insertMarker:(NSMenuItem *)sender {
    self->_action_controller->handle_action(action_kind::insert_marker);
}

- (IBAction)returnToZero:(NSMenuItem *)sender {
    self->_action_controller->handle_action(action_kind::return_to_zero);
}

- (IBAction)undo:(NSMenuItem *)sender {
    self->_action_controller->handle_action(action_kind::undo);
}

- (IBAction)redo:(NSMenuItem *)sender {
    self->_action_controller->handle_action(action_kind::redo);
}

- (IBAction)exportToFile:(id)sender {
    self->_action_controller->handle_action(action_kind::select_file_for_export);
}

- (IBAction)cut:(id)sender {
    self->_action_controller->handle_action(action_kind::cut);
}

- (IBAction)copy:(id)sender {
    self->_action_controller->handle_action(action_kind::copy);
}

- (IBAction)paste:(id)sender {
    self->_action_controller->handle_action(action_kind::paste);
}

#pragma mark -

- (BOOL)validateMenuItem:(NSMenuItem *)menuItem {
    if (auto const action = [self actionForSelector:menuItem.action]) {
        if (auto const ui_root_level = self->_root_level.lock()) {
            return ui_root_level->root->responds_to_action(action.value());
        }
    }
    return NO;
}

- (std::optional<action>)actionForSelector:(SEL)selector {
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
    } else if (selector == @selector(exportToFile:)) {
        return action_kind::select_file_for_export;
    } else if (selector == @selector(cut:)) {
        return action_kind::cut;
    } else if (selector == @selector(copy:)) {
        return action_kind::copy;
    } else if (selector == @selector(paste:)) {
        return action_kind::paste;
    }

    return std::nullopt;
}

#pragma mark -

- (void)showSelectFileForExportDialog {
    auto *const panel = [NSSavePanel savePanel];
    panel.canCreateDirectories = YES;
    panel.allowedContentTypes = @[UTTypeAudio];
    panel.nameFieldStringValue = @"Untitled";

    if ([panel runModal] == NSModalResponseOK) {
        auto const path = to_string((__bridge CFStringRef)panel.URL.path);
        self->_action_controller->handle_action({action_kind::export_to_file, path});
    }
}

- (void)showModuleNameSheetWithValue:(std::string const &)value {
    auto const splited = yas::split(value, ',');
    if (splited.size() < 2) {
        assert(0);
        return;
    }

    time::range const range{std::stoi(splited.at(0)), std::stoull(splited.at(1))};

    auto *const vc = [AEModuleNameViewController instantiateWithProjectId:self->_project_id moduleRange:range];

    auto *const unowned_self = [[YASUnownedObject<AEMetalViewController *> alloc] initWithObject:self];
    auto *const unowned_vc = [[YASUnownedObject<AEModuleNameViewController *> alloc] initWithObject:vc];

    [vc observe_event:[unowned_self, unowned_vc](auto const &) {
        auto *const self = unowned_self.object;
        auto *const vc = unowned_vc.object;

        [self dismissViewController:vc];

        self->_sheet_canceller = nullptr;
    }].end()
        ->set_to(self->_sheet_canceller);

    [self presentViewControllerAsSheet:vc];
}

@end
