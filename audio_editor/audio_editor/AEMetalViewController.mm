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
        controller->handle_action(action_kind::toggle_play);
    }
}

- (IBAction)nudgePrevious:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action_kind::nudge_previous);
    }
}

- (IBAction)nudgeNext:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action_kind::nudge_next);
    }
}

- (IBAction)rotateNudgingKind:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action_kind::rotate_nudging_next_unit);
    }
}

- (IBAction)rotateTimingFraction:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action_kind::rotate_timing_fraction);
    }
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

- (IBAction)dropHeadAndOffset:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action_kind::drop_head_and_offset);
    }
}

- (IBAction)split:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action_kind::split);
    }
}

- (IBAction)dropTailAndOffset:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action_kind::drop_tail_and_offset);
    }
}

- (IBAction)erase:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action(action_kind::erase_and_offset);
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

- (IBAction)goToMarker1:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action({action_kind::go_to_marker, "0"});
    }
}

- (IBAction)goToMarker2:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action({action_kind::go_to_marker, "1"});
    }
}

- (IBAction)goToMarker3:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action({action_kind::go_to_marker, "2"});
    }
}

- (IBAction)goToMarker4:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action({action_kind::go_to_marker, "3"});
    }
}

- (IBAction)goToMarker5:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action({action_kind::go_to_marker, "4"});
    }
}

- (IBAction)goToMarker6:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action({action_kind::go_to_marker, "5"});
    }
}

- (IBAction)goToMarker7:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action({action_kind::go_to_marker, "6"});
    }
}

- (IBAction)goToMarker8:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action({action_kind::go_to_marker, "7"});
    }
}

- (IBAction)goToMarker9:(NSMenuItem *)sender {
    if (auto const controller = self->_action_controller.lock()) {
        controller->handle_action({action_kind::go_to_marker, "8"});
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
        return self->_ui_root->responds_to_action(action.value());
    } else {
        return NO;
    }
}

- (std::optional<action>)actionForSelector:(SEL)selector {
    if (selector == @selector(togglePlay:)) {
        return action_kind::toggle_play;
    } else if (selector == @selector(nudgePrevious:)) {
        return action_kind::nudge_previous;
    } else if (selector == @selector(nudgeNext:)) {
        return action_kind::nudge_next;
    } else if (selector == @selector(rotateNudgingKind:)) {
        return action_kind::rotate_nudging_next_unit;
    } else if (selector == @selector(rotateTimingFraction:)) {
        return action_kind::rotate_timing_fraction;
    } else if (selector == @selector(jumpPrevious:)) {
        return action_kind::jump_previous;
    } else if (selector == @selector(jumpNext:)) {
        return action_kind::jump_next;
    } else if (selector == @selector(jumpToBeginning:)) {
        return action_kind::jump_to_beginning;
    } else if (selector == @selector(jumpToEnd:)) {
        return action_kind::jump_to_end;
    } else if (selector == @selector(dropHeadAndOffset:)) {
        return action_kind::drop_head_and_offset;
    } else if (selector == @selector(split:)) {
        return action_kind::split;
    } else if (selector == @selector(dropTailAndOffset:)) {
        return action_kind::drop_tail_and_offset;
    } else if (selector == @selector(erase:)) {
        return action_kind::erase_and_offset;
    } else if (selector == @selector(insertMarker:)) {
        return action_kind::insert_marker;
    } else if (selector == @selector(returnToZero:)) {
        return action_kind::return_to_zero;
    } else if (selector == @selector(goToMarker1:)) {
        return action{action_kind::go_to_marker, "0"};
    } else if (selector == @selector(goToMarker2:)) {
        return action{action_kind::go_to_marker, "1"};
    } else if (selector == @selector(goToMarker3:)) {
        return action{action_kind::go_to_marker, "2"};
    } else if (selector == @selector(goToMarker4:)) {
        return action{action_kind::go_to_marker, "3"};
    } else if (selector == @selector(goToMarker5:)) {
        return action{action_kind::go_to_marker, "4"};
    } else if (selector == @selector(goToMarker6:)) {
        return action{action_kind::go_to_marker, "5"};
    } else if (selector == @selector(goToMarker7:)) {
        return action{action_kind::go_to_marker, "6"};
    } else if (selector == @selector(goToMarker8:)) {
        return action{action_kind::go_to_marker, "7"};
    } else if (selector == @selector(goToMarker9:)) {
        return action{action_kind::go_to_marker, "8"};
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
