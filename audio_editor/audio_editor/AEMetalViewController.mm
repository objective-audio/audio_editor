//
//  AEMetalViewController.mm
//

#import "AEMetalViewController.h"
#include <audio_editor_core/audio_editor_core_umbrella.h>

using namespace yas;
using namespace yas::ae;

@interface AEMetalViewController () <NSMenuItemValidation>

@end

@implementation AEMetalViewController {
    std::shared_ptr<ui_root> _ui_root;
}

- (void)setupWithProjectID:(std::string const &)project_id {
    auto const metal_system = ui::metal_system::make_shared(
        objc_ptr_with_move_object(MTLCreateSystemDefaultDevice()).object(), self.metalView);
    auto const standard = ui::standard::make_shared([self view_look], metal_system);
    self->_ui_root = ui_root::make_shared(standard, project_view_presenter::make_shared(project_id));

    [self configure_with_metal_system:metal_system
                             renderer:standard->renderer()
                        event_manager:standard->event_manager()];
}

- (IBAction)togglePlay:(NSMenuItem *)sender {
    self->_ui_root->handle_action(action::toggle_play);
}

- (IBAction)nudgePrevious:(NSMenuItem *)sender {
    self->_ui_root->handle_action(action::nudge_previous);
}

- (IBAction)nudgeNext:(NSMenuItem *)sender {
    self->_ui_root->handle_action(action::nudge_next);
}

- (IBAction)jumpPrevious:(NSMenuItem *)sender {
    self->_ui_root->handle_action(action::jump_previous);
}

- (IBAction)jumpNext:(NSMenuItem *)sender {
    self->_ui_root->handle_action(action::jump_next);
}

- (IBAction)dropHeadAndOffset:(NSMenuItem *)sender {
    self->_ui_root->handle_action(action::drop_head_and_offset);
}

- (IBAction)split:(NSMenuItem *)sender {
    self->_ui_root->handle_action(action::split);
}

- (IBAction)dropTailAndOffset:(NSMenuItem *)sender {
    self->_ui_root->handle_action(action::drop_tail_and_offset);
}

- (IBAction)erase:(NSMenuItem *)sender {
    self->_ui_root->handle_action(action::erase);
}

- (IBAction)returnToZero:(NSMenuItem *)sender {
    self->_ui_root->handle_action(action::return_to_zero);
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
        return action::erase;
    } else if (selector == @selector(returnToZero:)) {
        return action::return_to_zero;
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
        case action::erase:
            return @selector(erase:);
        case action::return_to_zero:
            return @selector(returnToZero:);
    }
}

@end
