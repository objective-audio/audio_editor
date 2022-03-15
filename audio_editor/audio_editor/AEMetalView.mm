//
//  AEMetalView.mm
//

#import "AEMetalView.h"
#include <audio_editor_core/ae_action_controller.h>
#include <audio_editor_core/ae_action_utils.h>
#include <audio_editor_core/ae_context_menu_presenter.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_ui_button_utils.h>
#include <cpp_utils/yas_cf_utils.h>
#include <objc_utils/yas_objc_unowned.h>

using namespace yas;
using namespace yas::ae;

@interface AEMetalView () <NSMenuDelegate>
@end

@implementation AEMetalView {
    std::weak_ptr<context_menu_presenter> _context_menu_presenter;
    std::shared_ptr<action_controller> _action_controller;
    observing::canceller_pool _pool;
}

- (void)setupWithProjectID:(std::string const &)project_id {
    auto const &project_level = hierarchy::project_level_for_id(project_id);

    self->_context_menu_presenter = project_level->context_menu_presenter;
    self->_action_controller = action_controller::make_shared(project_id);

    auto *const unowned = [[YASUnownedObject<AEMetalView *> alloc] initWithObject:self];

    project_level->context_menu_presenter
        ->observe([unowned](std::optional<context_menu> const &event) {
            if (!event.has_value()) {
                return;
            }

            auto const &event_value = event.value();

            auto *const view = unowned.object;

            auto const position = ui_button_utils::to_position_from_event_string(event_value.position);
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
        })
        .end()
        ->add_to(self->_pool);
}

- (void)contextMenuItemClicked:(NSMenuItem *)menuItem {
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
    }
}

#pragma mark -

- (void)menuDidClose:(NSMenu *)menu {
    // タップされた時に先に呼ばれるので遅らせる
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
    }
}

@end
