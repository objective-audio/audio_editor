//
//  ae_modifiers_holder.mm
//

#include "ae_modifiers_holder.hpp"
#include <audio_editor_core/ae_modifier_utils.hpp>

using namespace yas;
using namespace yas::ae;

modifiers_holder::modifiers_holder(std::shared_ptr<ui::event_manager> const &event_manager)
    : _event_manager(event_manager), _modifier_notifier(observing::notifier<ae::modifier_event>::make_shared()) {
    this->_event_manager
        ->observe([this](std::shared_ptr<ui::event> const &event) {
            switch (event->type()) {
                case ui::event_type::modifier: {
                    auto const modifier_event = event->get<ui::modifier>();
                    auto const flags = modifier_event.flag;
                    auto const modifier = modifier_utils::to_modifier(flags);

                    switch (event->phase()) {
                        case ui::event_phase::began: {
                            if (!this->_modifiers.contains(modifier)) {
                                this->_modifiers.insert(modifier);
                                this->_modifier_notifier->notify(
                                    {.state = ae::modifier_event_state::began, .modifier = modifier});
                            }
                        } break;

                        case ui::event_phase::ended: {
                            if (this->_modifiers.contains(modifier)) {
                                this->_modifiers.erase(modifier);
                                this->_modifier_notifier->notify(
                                    {.state = ae::modifier_event_state::ended, .modifier = modifier});
                            }
                        } break;

                        default:
                            break;
                    }
                } break;

                case ui::event_type::key:
                case ui::event_type::cursor:
                case ui::event_type::touch:
                case ui::event_type::pinch:
                case ui::event_type::scroll:
                    break;
            }
        })
        .end()
        ->add_to(this->_pool);
}

std::unordered_set<ae::modifier> const &modifiers_holder::modifiers() const {
    return this->_modifiers;
}

observing::endable modifiers_holder::observe(std::function<void(ae::modifier_event const &)> &&handler) {
    return this->_modifier_notifier->observe(std::move(handler));
}
