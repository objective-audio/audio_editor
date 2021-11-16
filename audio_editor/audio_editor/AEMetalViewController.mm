//
//  AEMetalViewController.mm
//

#import "AEMetalViewController.h"
#import <UniformTypeIdentifiers/UTCoreTypes.h>
#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_pool.h>
#include <audio_editor_core/audio_editor_core_umbrella.h>

using namespace yas;
using namespace yas::ae;

@interface AEMetalViewController () <NSMenuItemValidation>

@end

@implementation AEMetalViewController {
    std::shared_ptr<ui_root> _ui_root;
    std::weak_ptr<action_controller> _action_controller;
    observing::canceller_pool _pool;
}

- (void)setupWithProjectID:(std::string const &)project_id {
    auto const metal_system = ui::metal_system::make_shared(
        objc_ptr_with_move_object(MTLCreateSystemDefaultDevice()).object(), self.metalView);
    auto const standard = ui::standard::make_shared([self view_look], metal_system);
    self->_ui_root = ui_root::make_shared(standard, project_id);

    auto const &project = app::global()->project_pool()->project_for_id(project_id);
    self->_action_controller = project->action_controller();

    [self configure_with_metal_system:metal_system
                             renderer:standard->renderer()
                        event_manager:standard->event_manager()];

    project->dialog_presenter()
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
}

- (IBAction)togglePlay:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action::toggle_play);
    }
}

- (IBAction)nudgePrevious:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action::nudge_previous);
    }
}

- (IBAction)nudgeNext:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action::nudge_next);
    }
}

- (IBAction)jumpPrevious:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action::jump_previous);
    }
}

- (IBAction)jumpNext:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action::jump_next);
    }
}

- (IBAction)dropHeadAndOffset:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action::drop_head_and_offset);
    }
}

- (IBAction)split:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action::split);
    }
}

- (IBAction)dropTailAndOffset:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action::drop_tail_and_offset);
    }
}

- (IBAction)erase:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action::erase_and_offset);
    }
}

- (IBAction)insertMarker:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action::insert_marker);
    }
}

- (IBAction)returnToZero:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action::return_to_zero);
    }
}

- (IBAction)goToMarker1:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action::go_to_marker_1);
    }
}

- (IBAction)goToMarker2:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action::go_to_marker_2);
    }
}

- (IBAction)goToMarker3:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action::go_to_marker_3);
    }
}

- (IBAction)goToMarker4:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action::go_to_marker_4);
    }
}

- (IBAction)goToMarker5:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action::go_to_marker_5);
    }
}

- (IBAction)goToMarker6:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action::go_to_marker_6);
    }
}

- (IBAction)goToMarker7:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action::go_to_marker_7);
    }
}

- (IBAction)goToMarker8:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action::go_to_marker_8);
    }
}

- (IBAction)goToMarker9:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action::go_to_marker_9);
    }
}

- (IBAction)undo:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action::undo);
    }
}

- (IBAction)redo:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action::redo);
    }
}

- (IBAction)exportToFile:(id)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action::select_file_for_export);
    }
}

#pragma mark -

- (BOOL)validateMenuItem:(NSMenuItem *)menuItem {
    if (auto const action = [self actionForSelector:menuItem.action]) {
        return self->_ui_root->responds_to_action(action.value());
    } else {
        return NO;
    }
}

- (std::optional<action>)actionForSelector:(SEL)selector {
    if (selector == @selector(togglePlay:)) {
        return action::toggle_play;
    } else if (selector == @selector(nudgePrevious:)) {
        return action::nudge_previous;
    } else if (selector == @selector(nudgeNext:)) {
        return action::nudge_next;
    } else if (selector == @selector(jumpPrevious:)) {
        return action::jump_previous;
    } else if (selector == @selector(jumpNext:)) {
        return action::jump_next;
    } else if (selector == @selector(dropHeadAndOffset:)) {
        return action::drop_head_and_offset;
    } else if (selector == @selector(split:)) {
        return action::split;
    } else if (selector == @selector(dropTailAndOffset:)) {
        return action::drop_tail_and_offset;
    } else if (selector == @selector(erase:)) {
        return action::erase_and_offset;
    } else if (selector == @selector(insertMarker:)) {
        return action::insert_marker;
    } else if (selector == @selector(returnToZero:)) {
        return action::return_to_zero;
    } else if (selector == @selector(goToMarker1:)) {
        return action::go_to_marker_1;
    } else if (selector == @selector(goToMarker2:)) {
        return action::go_to_marker_2;
    } else if (selector == @selector(goToMarker3:)) {
        return action::go_to_marker_3;
    } else if (selector == @selector(goToMarker4:)) {
        return action::go_to_marker_4;
    } else if (selector == @selector(goToMarker5:)) {
        return action::go_to_marker_5;
    } else if (selector == @selector(goToMarker6:)) {
        return action::go_to_marker_6;
    } else if (selector == @selector(goToMarker7:)) {
        return action::go_to_marker_7;
    } else if (selector == @selector(goToMarker8:)) {
        return action::go_to_marker_8;
    } else if (selector == @selector(goToMarker9:)) {
        return action::go_to_marker_9;
    } else if (selector == @selector(undo:)) {
        return action::undo;
    } else if (selector == @selector(redo:)) {
        return action::redo;
    } else if (selector == @selector(exportToFile:)) {
        return action::select_file_for_export;
    }

    return std::nullopt;
}

- (SEL)selectorForAction:(action const)action {
    switch (action) {
        case action::toggle_play:
            return @selector(togglePlay:);
        case action::nudge_previous:
            return @selector(nudgePrevious:);
        case action::nudge_next:
            return @selector(nudgeNext:);
        case action::jump_previous:
            return @selector(jumpPrevious:);
        case action::jump_next:
            return @selector(jumpNext:);
        case action::drop_head_and_offset:
            return @selector(dropHeadAndOffset:);
        case action::split:
            return @selector(split:);
        case action::drop_tail_and_offset:
            return @selector(dropTailAndOffset:);
        case action::erase_and_offset:
            return @selector(erase:);
        case action::insert_marker:
            return @selector(insertMarker:);
        case action::return_to_zero:
            return @selector(returnToZero:);
        case action::go_to_marker_1:
            return @selector(goToMarker1:);
        case action::go_to_marker_2:
            return @selector(goToMarker2:);
        case action::go_to_marker_3:
            return @selector(goToMarker3:);
        case action::go_to_marker_4:
            return @selector(goToMarker4:);
        case action::go_to_marker_5:
            return @selector(goToMarker5:);
        case action::go_to_marker_6:
            return @selector(goToMarker6:);
        case action::go_to_marker_7:
            return @selector(goToMarker7:);
        case action::go_to_marker_8:
            return @selector(goToMarker8:);
        case action::go_to_marker_9:
            return @selector(goToMarker9:);
        case action::undo:
            return @selector(undo:);
        case action::redo:
            return @selector(redo:);
        case action::select_file_for_export:
            return @selector(exportToFile:);
    }
}

@end
