//
//  AEMetalView.mm
//

#import "AEMetalView.h"
#include <audio_editor_core/ae_action_utils.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_action_controller.h>
#include <audio_editor_core/ae_project_modal_lifecycle.h>
#include <audio_editor_core/ae_ui_event_utils.h>
#include <cpp_utils/yas_assertion.h>
#include <cpp_utils/yas_cf_utils.h>
#include <cpp_utils/yas_unowned.h>

using namespace yas;
using namespace yas::ae;

@interface AEMetalView () <NSMenuDelegate>
@end

@implementation AEMetalView {
    std::weak_ptr<project_modal_lifecycle> _lifecycle;
    std::optional<context_menu> _shown_context_menu;
    std::weak_ptr<project_action_controller> _action_controller;
    observing::canceller_pool _pool;
}

- (void)setupWithProjectID:(project_id const &)project_id {
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(project_id);
    auto const &lifecycle = project_lifetime->modal_lifecycle;

    [self setupWithLifecycle:lifecycle actionController:project_lifetime->action_controller];
}

- (void)setupWithLifecycle:(std::shared_ptr<project_modal_lifecycle> const &)lifecycle
          actionController:(std::shared_ptr<project_action_controller> const &)action_controller {
    self->_lifecycle = lifecycle;
    self->_action_controller = action_controller;

    auto *const unowned = make_unowned(self);

    lifecycle
        ->observe([unowned](std::optional<project_sub_lifetime> const &sub_lifetime) {
            if (auto const &lifetime = get<context_menu_lifetime>(sub_lifetime)) {
                auto *const view = unowned.object;
                [view showContextMenu:lifetime->context_menu];
            }
        })
        .end()
        ->add_to(self->_pool);
}

- (void)showContextMenu:(ae::context_menu const &)context_menu {
    self->_shown_context_menu = context_menu;

    auto const position = ui_event_utils::to_position_from_event_string(context_menu.position);
    auto const view_location = [self view_location_from_ui_position:position];

    auto *const menu = [[NSMenu alloc] initWithTitle:@""];
    menu.delegate = self;

    NSInteger idx = 0;

    for (auto const &action : context_menu.actions) {
        if (action) {
            auto *const item = [[NSMenuItem alloc]
                initWithTitle:(__bridge NSString *)to_cf_object(action_utils::to_context_menu_title(action.value()))
                       action:@selector(contextMenuItemClicked:)
                keyEquivalent:@""];
            item.tag = idx;
            [menu addItem:item];
        } else {
            [menu addItem:[NSMenuItem separatorItem]];
        }

        ++idx;
    }

    NSEvent *popUpEvent = [NSEvent otherEventWithType:NSEventTypeApplicationDefined
                                             location:NSMakePoint(view_location.x, view_location.y)
                                        modifierFlags:kNilOptions
                                            timestamp:0
                                         windowNumber:self.window.windowNumber
                                              context:nil
                                              subtype:0
                                                data1:0
                                                data2:0];

    [NSMenu popUpContextMenu:menu withEvent:popUpEvent forView:self];
}

- (void)contextMenuItemClicked:(NSMenuItem *)menuItem {
    auto const lifecycle = self->_lifecycle.lock();
    if (!lifecycle) {
        assertion_failure_if_not_test();
        return;
    }

    auto const &lifetime = lifecycle->context_menu_lifetime();
    if (!lifetime) {
        assertion_failure_if_not_test();
        return;
    }

    auto const &context_menu = lifetime->context_menu;
    auto const &idx = menuItem.tag;
    if (idx < context_menu.actions.size()) {
        if (auto const &action = context_menu.actions.at(idx)) {
            if (auto const action_controller = self->_action_controller.lock()) {
                action_controller->handle_action(action.value());
            }
        }
    }

    self->_shown_context_menu = std::nullopt;
}

#pragma mark -

- (void)menuDidClose:(NSMenu *)menu {
    __weak typeof(self) wself = self;
    // タップされた時にボタンのアクションより先に呼ばれるので遅らせる
    dispatch_async(dispatch_get_main_queue(), [wself] {
        if (!wself) {
            assertion_failure_if_not_test();
            return;
        }

        AEMetalView *self = wself;

        auto const lifecycle = self->_lifecycle.lock();
        if (!lifecycle) {
            assertion_failure_if_not_test();
            return;
        }

        lifecycle->remove_context_menu();
        self->_shown_context_menu = std::nullopt;
    });
}

@end
