//
//  AEMetalViewController.mm
//

#import "AEMetalViewController.h"
#import <UniformTypeIdentifiers/UTCoreTypes.h>
#include <audio_editor_core/ae_action_controller.h>
#include <audio_editor_core/ae_app_level.h>
#include <audio_editor_core/ae_dialog_presenter.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_level.h>
#include <audio_editor_core/ae_project_level_pool.h>
#include <audio_editor_core/ae_ui_pool.h>
#include <audio_editor_core/audio_editor_core_umbrella.h>
#import "AEMetalView.h"

using namespace yas;
using namespace yas::ae;

@interface AEMetalViewController () <NSMenuItemValidation>

@end

@implementation AEMetalViewController {
    std::weak_ptr<ui_root> _ui_root;
    std::weak_ptr<action_controller> _action_controller;
    std::weak_ptr<context_menu_presenter> _context_menu_presenter;
    observing::canceller_pool _pool;
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

- (void)dealloc {
    app_level::global()->ui_pool->remove_ui_root_for_view_id(self.project_view_id);
}

- (void)setupWithProjectID:(std::string const &)project_id {
    auto const metal_system = ui::metal_system::make_shared(
        objc_ptr_with_move_object(MTLCreateSystemDefaultDevice()).object(), self.metalView);
    auto const standard = ui::standard::make_shared([self view_look], metal_system);
    self->_ui_root = app_level::global()->ui_pool->add_and_return_ui_root(standard, project_id, self.project_view_id);

    auto const &project_level = app_level::global()->project_pool->project_level_for_id(project_id);
    self->_action_controller = project_level->action_controller;

    [self configure_with_metal_system:metal_system
                             renderer:standard->renderer()
                        event_manager:standard->event_manager()];

    project_level->dialog_presenter
        ->observe_event([weak_controller = self->_action_controller](dialog_event const &event) {
            auto const panel = [NSSavePanel savePanel];
            panel.canCreateDirectories = YES;
            panel.allowedFileTypes = @[@"wav"];
            panel.nameFieldStringValue = @"Untitled";

            if ([panel runModal] == NSModalResponseOK) {
                auto const path = to_string((__bridge CFStringRef)panel.URL.path);
                auto const url = url::file_url(path);

                if (auto const controller = weak_controller.lock()) {
                    controller->export_to_file(url);
                }
            }
        })
        .end()
        ->add_to(self->_pool);

    [self.aeMetalView setupWithProjectID:project_id];
}

- (IBAction)jumpPrevious:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action_kind::jump_previous);
    }
}

- (IBAction)jumpNext:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action_kind::jump_next);
    }
}

- (IBAction)jumpToBeginning:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action_kind::jump_to_beginning);
    }
}

- (IBAction)jumpToEnd:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action_kind::jump_to_end);
    }
}

- (IBAction)insertMarker:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action_kind::insert_marker);
    }
}

- (IBAction)returnToZero:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action_kind::return_to_zero);
    }
}

- (IBAction)undo:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action_kind::undo);
    }
}

- (IBAction)redo:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action_kind::redo);
    }
}

- (IBAction)exportToFile:(id)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action_kind::select_file_for_export);
    }
}

- (IBAction)cut:(id)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action_kind::cut);
    }
}

- (IBAction)copy:(id)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action_kind::copy);
    }
}

- (IBAction)paste:(id)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action_kind::paste);
    }
}

#pragma mark -

- (BOOL)validateMenuItem:(NSMenuItem *)menuItem {
    if (auto const action = [self actionForSelector:menuItem.action]) {
        if (auto const ui_root = self->_ui_root.lock()) {
            return ui_root->responds_to_action(action.value());
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

@end
