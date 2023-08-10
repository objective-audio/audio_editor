//
//  ae_ui_range_selection_input.mm
//

#include "ae_ui_range_selection_input.hpp"
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <cpp_utils/yas_lock.h>
#include <audio_editor_core/ae_deselector.hpp>
#include <audio_editor_core/ae_modifiers_holder.hpp>
#include <audio_editor_core/ae_range_selector.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_range_selection_input> ui_range_selection_input::make_shared(
    project_lifetime_id const &project_lifetime_id, ui::node *node) {
    auto const &project_editing_lifetime = hierarchy::project_editing_lifetime_for_id(project_lifetime_id);
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_project_lifetime_id(project_lifetime_id);
    return std::make_shared<ui_range_selection_input>(
        resource_lifetime->standard, node, project_editing_lifetime->range_selector,
        project_editing_lifetime->deselector, resource_lifetime->modifiers_holder);
}

ui_range_selection_input::ui_range_selection_input(std::shared_ptr<ui::standard> const &standard, ui::node *node,
                                                   std::shared_ptr<range_selector> const &range_selector,
                                                   std::shared_ptr<deselector> const &deselector,
                                                   std::shared_ptr<modifiers_holder> const &modifiers_holder)
    : _collider_node(ui::node::make_shared()),
      _collider(ui::collider::make_shared(ui::shape::make_shared(ui::anywhere_shape{}))),
      _touch_tracker(ui::touch_tracker::make_shared(standard, this->_collider_node)),
      _range_selector(range_selector),
      _deselector(deselector),
      _modifiers_holder(modifiers_holder) {
    this->_collider_node->set_colliders({this->_collider});

    node->add_sub_node(this->_collider_node);

    this->_touch_tracker
        ->observe([this](const ui::touch_tracker::context &context) {
            if (auto const locked = yas::lock(this->_range_selector, this->_deselector, this->_modifiers_holder);
                fulfilled(locked)) {
                auto const &[range_selector, deselector, modifiers_holder] = locked;
                if (context.phase == ui::touch_tracker_phase::began) {
                    auto const &modifiers = modifiers_holder->modifiers();
                    if (modifiers.empty()) {
                        deselector->deselect_all();
                        range_selector->begin(context.touch_event.position);
                    } else if (modifiers.size() == 1 && modifiers.contains(modifier::shift)) {
                        range_selector->begin(context.touch_event.position);
                    }
                }
            }
        })
        .end()
        ->add_to(this->_pool);

    standard->event_manager()
        ->observe([this](std::shared_ptr<ui::event> const &event) {
            if (event->type() == ui::event_type::touch) {
                if (auto const range_selector = this->_range_selector.lock()) {
                    auto const touch = event->get<ui::touch>();

                    switch (event->phase()) {
                        case ui::event_phase::began:
                            // touch_trackerから開始する
                            break;
                        case ui::event_phase::changed:
                            range_selector->move(touch.position);
                            break;
                        case ui::event_phase::ended:
                        case ui::event_phase::canceled:
                            range_selector->move(touch.position);
                            range_selector->end();
                            break;
                        case ui::event_phase::stationary:
                        case ui::event_phase::may_begin:
                        case ui::event_phase::none:
                            break;
                    }
                }
            }
        })
        .end()
        ->add_to(this->_pool);
}
