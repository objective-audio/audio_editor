//
//  AEMetalViewController.mm
//

#import "AEMetalViewController.h"
#import <UniformTypeIdentifiers/UTCoreTypes.h>
#include <audio_editor_core/ae_project_action_controller.h>
#include <audio_editor_core/ae_project_sub_level_router.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_root.h>
#include <audio_editor_core/ae_ui_root_level.h>
#include <audio_editor_core/ae_ui_root_level_router.h>
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
    project_id _project_id;
    std::weak_ptr<ui_root_level> _root_level;
    std::weak_ptr<project_sub_level_router> _project_sub_level_router;
    std::weak_ptr<project_action_controller> _action_controller;
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
    auto const &ui_root_level_router = hierarchy::app_level()->ui_root_level_router;
    auto const &project_level = hierarchy::project_level_for_id(project_id);

    [self setupWithProjectID:project_id
            uiRootLevelRouter:ui_root_level_router
             actionController:project_level->action_controller
        projectSubLevelRouter:project_level->sub_level_router];
}

- (void)setupWithProjectID:(project_id const &)project_id
         uiRootLevelRouter:(std::shared_ptr<ae::ui_root_level_router> const &)ui_root_level_router
          actionController:(std::shared_ptr<project_action_controller> const &)action_controller
     projectSubLevelRouter:(std::shared_ptr<project_sub_level_router> const &)project_sub_level_router {
    self->_project_id = project_id;

    auto const metal_system = ui::metal_system::make_shared(
        objc_ptr_with_move_object(MTLCreateSystemDefaultDevice()).object(), self.metalView);
    auto const standard = ui::standard::make_shared([self view_look], metal_system);

    ui_root_level_router->add_level(standard, {.project_id = project_id, .view_id = self.project_view_id});
    self->_root_level = ui_root_level_router->level_for_view_id(self.project_view_id);

    self->_action_controller = action_controller;
    self->_project_sub_level_router = project_sub_level_router;

    [self configure_with_metal_system:metal_system
                             renderer:standard->renderer()
                        event_manager:standard->event_manager()];

    auto *const unowned_self = make_unowned(self);

    project_sub_level_router
        ->observe([unowned_self](std::optional<project_sub_level> const &sub_level) {
            auto *const self = unowned_self.object;

            if (!sub_level.has_value()) {
                [self hideModal];
            } else if (auto const &level = get_level<sheet_level>(sub_level)) {
                switch (level->content.kind) {
                    case sheet_kind::module_name:
                        [self showModuleNameSheetWithValue:level->content.value];
                        break;
                }
            } else if (auto const &level = get_level<dialog_level>(sub_level)) {
                switch (level->content) {
                    case dialog_content::select_file_for_import:
                        [self showSelectFileForImportDialog];
                        break;
                    case dialog_content::select_file_for_export:
                        [self showSelectFileForExportDialog];
                        break;
                }
            }
        })
        .end()
        ->add_to(self->_pool);

    [self.aeMetalView setupWithProjectID:project_id];
}

- (IBAction)jumpPrevious:(NSMenuItem *)sender {
    if (auto const action_controller = self->_action_controller.lock()) {
        action_controller->handle_action(action_kind::jump_previous);
    }
}

- (IBAction)jumpNext:(NSMenuItem *)sender {
    if (auto const action_controller = self->_action_controller.lock()) {
        action_controller->handle_action(action_kind::jump_next);
    }
}

- (IBAction)jumpToBeginning:(NSMenuItem *)sender {
    if (auto const action_controller = self->_action_controller.lock()) {
        action_controller->handle_action(action_kind::jump_to_beginning);
    }
}

- (IBAction)jumpToEnd:(NSMenuItem *)sender {
    if (auto const action_controller = self->_action_controller.lock()) {
        action_controller->handle_action(action_kind::jump_to_end);
    }
}

- (IBAction)insertMarker:(NSMenuItem *)sender {
    if (auto const action_controller = self->_action_controller.lock()) {
        action_controller->handle_action(action_kind::insert_marker);
    }
}

- (IBAction)returnToZero:(NSMenuItem *)sender {
    if (auto const action_controller = self->_action_controller.lock()) {
        action_controller->handle_action(action_kind::return_to_zero);
    }
}

- (IBAction)undo:(NSMenuItem *)sender {
    if (auto const action_controller = self->_action_controller.lock()) {
        action_controller->handle_action(action_kind::undo);
    }
}

- (IBAction)redo:(NSMenuItem *)sender {
    if (auto const action_controller = self->_action_controller.lock()) {
        action_controller->handle_action(action_kind::redo);
    }
}

- (IBAction)importFromFile:(id)sender {
    if (auto const action_controller = self->_action_controller.lock()) {
        action_controller->handle_action(action_kind::select_file_for_import);
    }
}

- (IBAction)exportToFile:(id)sender {
    if (auto const action_controller = self->_action_controller.lock()) {
        action_controller->handle_action(action_kind::select_file_for_export);
    }
}

- (IBAction)cut:(id)sender {
    if (auto const action_controller = self->_action_controller.lock()) {
        action_controller->handle_action(action_kind::cut);
    }
}

- (IBAction)copy:(id)sender {
    if (auto const action_controller = self->_action_controller.lock()) {
        action_controller->handle_action(action_kind::copy);
    }
}

- (IBAction)paste:(id)sender {
    if (auto const action_controller = self->_action_controller.lock()) {
        action_controller->handle_action(action_kind::paste);
    }
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
    }

    return std::nullopt;
}

#pragma mark -

- (void)showSelectFileForImportDialog {
    auto *const panel = [NSOpenPanel openPanel];
    panel.allowedContentTypes = @[UTTypeAudio];

    auto *const unowned_self = make_unowned(self);
    auto *const unowned_panel = make_unowned(panel);

    [panel beginWithCompletionHandler:[unowned_self, unowned_panel](NSModalResponse result) {
        auto *const self = unowned_self.object;
        auto *const panel = unowned_panel.object;

        if (auto const router = self->_project_sub_level_router.lock()) {
            router->remove_dialog();
        } else {
            assertion_failure_if_not_test();
        }

        if (result == NSModalResponseOK) {
            auto const path = to_string((__bridge CFStringRef)panel.URL.path);
            if (auto const action_controller = self->_action_controller.lock()) {
                action_controller->handle_action({action_kind::import_from_file, path});
            }
        }
    }];
}

- (void)showSelectFileForExportDialog {
    auto *const panel = [NSSavePanel savePanel];
    panel.canCreateDirectories = YES;
    panel.allowedContentTypes = @[UTTypeAudio];
    panel.nameFieldStringValue = @"Untitled";

    auto *const unowned_self = make_unowned(self);
    auto *const unowned_panel = make_unowned(panel);

    [panel beginWithCompletionHandler:[unowned_self, unowned_panel](NSModalResponse result) {
        auto *const self = unowned_self.object;
        auto *const panel = unowned_panel.object;

        if (auto const router = self->_project_sub_level_router.lock()) {
            router->remove_dialog();
        } else {
            assertion_failure_if_not_test();
        }

        if (result == NSModalResponseOK) {
            auto const path = to_string((__bridge CFStringRef)panel.URL.path);
            if (auto const action_controller = self->_action_controller.lock()) {
                action_controller->handle_action({action_kind::export_to_file, path});
            }
        }
    }];
}

- (void)showModuleNameSheetWithValue:(std::string const &)value {
    auto const splited = yas::split(value, ',');
    if (splited.size() < 2) {
        assert(0);
        return;
    }

    time::range const range{std::stoi(splited.at(0)), std::stoull(splited.at(1))};

    auto *const vc = [AEModuleNameViewController instantiateWithProjectId:self->_project_id moduleRange:range];

    [self presentViewControllerAsSheet:vc];
}

- (void)hideModal {
    for (NSViewController *modal in self.presentedViewControllers) {
        [self dismissViewController:modal];
    }
}

@end
