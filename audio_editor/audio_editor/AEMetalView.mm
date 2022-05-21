//
//  AEMetalView.mm
//

#import "AEMetalView.h"
#include <audio_editor_core/ae_action_controller.h>
#include <audio_editor_core/ae_action_utils.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_sub_level_router.h>
#include <audio_editor_core/ae_ui_event_utils.h>
#include <cpp_utils/yas_assertion.h>
#include <cpp_utils/yas_cf_utils.h>
#include <objc_utils/yas_objc_unowned.h>

using namespace yas;
using namespace yas::ae;

@interface AEMetalView () <NSMenuDelegate>
@end

@implementation AEMetalView {
    std::weak_ptr<project_sub_level_router> _router;
    std::shared_ptr<action_controller> _action_controller;
    observing::canceller_pool _pool;
}

- (void)setupWithProjectID:(project_id const &)project_id {
    auto const &project_level = hierarchy::project_level_for_id(project_id);

    auto const &router = project_level->sub_level_router;
    self->_router = router;
    self->_action_controller = action_controller::make_shared(project_id);

    auto *const unowned = [[YASUnownedObject<AEMetalView *> alloc] initWithObject:self];

    router
        ->observe([unowned](std::optional<project_sub_level> const &sub_level) {
#warning todo
            /*
                if (!event.has_value()) {
                    return;
                }

                auto const &event_value = event.value();

                auto *const view = unowned.object;

                auto const position = ui_event_utils::to_position_from_event_string(event_value.position);
                auto const view_location = [view view_location_from_ui_position:position];

                auto *const menu = [[NSMenu alloc] initWithTitle:@""];
                menu.delegate = view;

                NSInteger idx = 0;

                for (auto const &action : event_value.actions) {
                    if (action) {
                        auto *const item =
                            [[NSMenuItem alloc] initWithTitle:(__bridge NSString *)to_cf_object(
                                                                  action_utils::to_context_menu_title(action.value()))
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
                                                     windowNumber:view.window.windowNumber
                                                          context:nil
                                                          subtype:0
                                                            data1:0
                                                            data2:0];

                [NSMenu popUpContextMenu:menu withEvent:popUpEvent forView:view];
             */
        })
        .end()
        ->add_to(self->_pool);
}

- (void)contextMenuItemClicked:(NSMenuItem *)menuItem {
#warning todo
    /*
    if (auto const presenter = self->_context_menu_presenter.lock()) {
        if (auto const &context_menu = presenter->context_menu()) {
            auto const &context_menu_value = context_menu.value();
            auto const &idx = menuItem.tag;
            if (idx < context_menu_value.actions.size()) {
                if (auto const &action = context_menu_value.actions.at(idx)) {
                    self->_action_controller->handle_action(action.value());
                }
            }
        }

        presenter->set_context_menu(std::nullopt);
    }*/
}

#pragma mark -

- (void)menuDidClose:(NSMenu *)menu {
    __weak typeof(self) wself = self;
    // タップされた時に先に呼ばれるので遅らせる
    dispatch_async(dispatch_get_main_queue(), [wself] {
        if (!wself) {
            assertion_failure_if_not_test();
            return;
        }

        AEMetalView *self = wself;

        auto const router = self->_router.lock();
        if (!router) {
            assertion_failure_if_not_test();
            return;
        }

#warning todo
    });
    /*
    if (auto const presenter = self->_context_menu_presenter.lock()) {
        if (auto const context_menu = presenter->context_menu()) {
            dispatch_async(dispatch_get_main_queue(), [weak_presenter = to_weak(presenter), context_menu] {
                if (auto const presenter = weak_presenter.lock()) {
                    if (presenter->context_menu() == context_menu) {
                        presenter->set_context_menu(std::nullopt);
                    }
                }
            });
        }
    }*/
}

@end
