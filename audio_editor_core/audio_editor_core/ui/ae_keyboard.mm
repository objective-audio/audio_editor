//
//  ae_keyboard.mm
//

#include "ae_keyboard.h"
#include <audio_editor_core/ae_keyboard_utils.h>

using namespace yas;
using namespace yas::ae;

keyboard::keyboard(std::shared_ptr<ui::event_manager> const &manager)
    : _event_manager(manager), _notifier(observing::notifier<ae::key>::make_shared()) {
    this->_event_manager
        ->observe([this](std::shared_ptr<ui::event> const &event) {
            switch (event->type()) {
                case ui::event_type::modifier: {
                    auto const modifier_event = event->get<ui::modifier>();

                    switch (event->phase()) {
                        case ui::event_phase::began: {
                            this->_modifiers.insert(modifier_event.flag());
                        } break;
                        case ui::event_phase::ended: {
                            this->_modifiers.erase(modifier_event.flag());
                        } break;

                        default:
                            break;
                    }
                } break;

                case ui::event_type::key: {
                    if (event->phase() == ui::event_phase::ended && this->_modifiers.empty()) {
                        auto const key_event = event->get<ui::key>();
                        if (auto const key = keyboard_utils::to_key(key_event.key_code())) {
                            this->_notifier->notify(key.value());
                        }
                    }
                } break;

                case ui::event_type::cursor:
                case ui::event_type::touch:
                    break;
            }
        })
        .end()
        ->add_to(this->_pool);
}

observing::endable keyboard::observe(std::function<void(ae::key const &)> &&handler) {
    return this->_notifier->observe(std::move(handler));
}

std::shared_ptr<keyboard> keyboard::make_shared(std::shared_ptr<ui::event_manager> const &manager) {
    return std::shared_ptr<keyboard>(new keyboard{manager});
}
