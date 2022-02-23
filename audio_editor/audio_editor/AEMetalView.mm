//
//  AEMetalView.mm
//

#import "AEMetalView.h"
#include <audio_editor_core/ae_action_utils.h>
#include <audio_editor_core/ae_app_level.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_pool.h>
#include <objc_utils/yas_objc_unowned.h>

using namespace yas;
using namespace yas::ae;

@interface AEMetalView () <NSMenuDelegate>
@end

@implementation AEMetalView {
    std::weak_ptr<context_menu_presenter> _context_menu_presenter;
    std::weak_ptr<action_controller> _action_controller;
    observing::canceller_pool _pool;
}

- (void)setupWithProjectID:(std::string const &)project_id {
    auto const &project = app_level::global()->project_pool->project_for_id(project_id);

    self->_context_menu_presenter = project->context_menu_presenter();
    self->_action_controller = project->action_controller();

    auto *const unowned = [[YASUnownedObject<AEMetalView *> alloc] initWithObject:self];

    project->context_menu_presenter()
        ->observe([unowned](std::optional<context_menu> const &event) {
            if (!event.has_value()) {
                return;
            }

            auto const &event_value = event.value();

            auto *const view = unowned.object;

            auto const location = NSMakePoint(event_value.location_in_view.x, event_value.location_in_view.y);

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
                                                     location:location
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
        if (auto const controller = self->_action_controller.lock()) {
            if (auto const &context_menu = presenter->context_menu()) {
                auto const &context_menu_value = context_menu.value();
                auto const &idx = menuItem.tag;
                if (idx < context_menu_value.actions.size()) {
                    if (auto const &action = context_menu_value.actions.at(idx)) {
                        controller->handle_action(action.value());
                    }
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
