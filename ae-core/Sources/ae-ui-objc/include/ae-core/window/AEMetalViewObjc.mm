//
//  AEMetalView.mm
//

#import "AEMetalViewObjc.h"
#include <ae-core/global/ae_hierarchy.h>
#include <ae-core/global/utils/ae_action_utils.h>
#include <ae-core/global/value_types/ae_project_lifetime_id+objc.h>
#include <ae-core/project_editing/features/ae_project_action_sender.h>
#include <ae-core/project_editing/lifecycles/ae_project_modal_lifecycle.h>
#include <ae-core/ui_base/utils/ae_ui_event_utils.h>
#include <cpp-utils/assertion.h>
#include <cpp-utils/cf_utils.h>
#include <cpp-utils/unowned.h>
#include <ui/metal/view/yas_ui_metal_view.h>

using namespace yas;
using namespace yas::ae;

@interface AEMetalViewObjc () <NSMenuDelegate>
@end

@implementation AEMetalViewObjc {
    std::weak_ptr<project_modal_lifecycle> _lifecycle;
    std::optional<std::pair<project_sub_lifetime_id, context_menu>> _shown_context_menu;
    std::weak_ptr<project_action_sender> _action_sender;
    observing::canceller_pool _pool;
}

- (void)setupWithProjectLifetimeId:(ProjectLifetimeId *)lifetimeId {
    [self setupWithProjectLifetimeID:[lifetimeId raw_value]];
}

- (void)setupWithProjectLifetimeID:(project_lifetime_id const &)project_lifetime_id {
    auto const &project_editing_lifetime = hierarchy::project_editing_lifetime_for_id(project_lifetime_id);
    auto const &lifecycle = project_editing_lifetime->modal_lifecycle;

    [self setupWithLifecycle:lifecycle actionController:project_editing_lifetime->action_sender];
}

- (void)setupWithLifecycle:(std::shared_ptr<project_modal_lifecycle> const &)lifecycle
          actionController:(std::shared_ptr<project_action_sender> const &)action_sender {
    self->_lifecycle = lifecycle;
    self->_action_sender = action_sender;

    auto *const unowned = make_unowned(self);

    lifecycle
        ->observe([unowned](std::optional<project_modal_sub_lifetime> const &sub_lifetime) {
            using kind = project_modal_sub_lifetime_kind;

            switch (to_kind(sub_lifetime)) {
                case kind::context_menu: {
                    auto *const view = unowned.object;
                    auto const &lifetime = get<context_menu_lifetime>(sub_lifetime);
                    [view showContextMenu:{lifetime->lifetime_id, lifetime->context_menu}];
                } break;

                case kind::none:
                case kind::time_editing:
                case kind::file_import_dialog:
                case kind::file_export_dialog:
                case kind::dialog:
                case kind::module_name_sheet:
                case kind::marker_name_sheet:
                    break;
            }
        })
        .end()
        ->add_to(self->_pool);
}

- (void)showContextMenu:(std::pair<project_sub_lifetime_id, context_menu> const &)context_menu {
    self->_shown_context_menu = context_menu;

    auto const position = ui_event_utils::to_position_from_event_string(context_menu.second.position);
    auto const view_location = [self view_location_from_ui_position:position];

    auto *const menu = [[NSMenu alloc] initWithTitle:@""];
    menu.delegate = self;

    NSInteger idx = 0;

    for (auto const &action : context_menu.second.actions) {
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
            if (auto const action_sender = self->_action_sender.lock()) {
                action_sender->send(action.value());
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

        AEMetalViewObjc *self = wself;

        auto const lifecycle = self->_lifecycle.lock();
        if (!lifecycle) {
            assertion_failure_if_not_test();
            return;
        }

        if (!self->_shown_context_menu.has_value()) {
            assertion_failure_if_not_test();
            return;
        }

        auto const &lifetime_id = self->_shown_context_menu.value().first;
        lifecycle->remove_context_menu(lifetime_id);
        self->_shown_context_menu = std::nullopt;
    });
}

@end
