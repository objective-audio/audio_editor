//
//  ae_keyboard.mm
//

#include "ae_keyboard.h"
#include <audio_editor_core/ae_keyboard_utils.h>

using namespace yas;
using namespace yas::ae;

keyboard::keyboard(std::shared_ptr<ui::event_manager> const &manager)
    : _event_manager(manager), _key_notifier(observing::notifier<ae::key>::make_shared()), _modifier_notifier(observing::notifier<ae::modifier_event>::make_shared()) {
    this->_event_manager
        ->observe([this](std::shared_ptr<ui::event> const &event) {
            switch (event->type()) {
                case ui::event_type::modifier: {
                    auto const modifier_event = event->get<ui::modifier>();
                    auto const flags = modifier_event.flag();

                    switch (event->phase()) {
                        case ui::event_phase::began: {
                            if (!this->_modifiers.contains(flags)) {
                                this->_modifiers.insert(flags);
                                this->_modifier_notifier->notify({.state = ae::modifier_event_state::began,
                                                                  .modifier = keyboard_utils::to_modifier(flags)});
                            }
                        } break;

                        case ui::event_phase::ended: {
                            if (this->_modifiers.contains(flags)) {
                                this->_modifiers.erase(modifier_event.flag());
                                this->_modifier_notifier->notify({.state = ae::modifier_event_state::ended,
                                                                  .modifier = keyboard_utils::to_modifier(flags)});
                            }
                        } break;

                        default:
                            break;
                    }
                } break;

                case ui::event_type::key: {
                    if (event->phase() == ui::event_phase::ended && this->_modifiers.empty()) {
                        auto const key_event = event->get<ui::key>();
                        if (auto const key = keyboard_utils::to_key(key_event.key_code())) {
                            this->_key_notifier->notify(key.value());
                        }
                    }
                } break;

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

observing::endable keyboard::observe_key(std::function<void(ae::key const &)> &&handler) {
    return this->_key_notifier->observe(std::move(handler));
}

observing::endable keyboard::observe_modifier(std::function<void(ae::modifier_event const &)> &&handler) {
    return this->_modifier_notifier->observe(std::move(handler));
}

std::shared_ptr<keyboard> keyboard::make_shared(std::shared_ptr<ui::event_manager> const &manager) {
    return std::shared_ptr<keyboard>(new keyboard{manager});
}
