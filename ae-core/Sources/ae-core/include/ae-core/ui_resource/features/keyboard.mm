//
//  keyboard.mm
//

#include "keyboard.h"
#include <ae-core/ui_resource/features/keyboard_utils.h>
#include <ae-core/ui_resource/features/modifier_utils.hpp>
#include <ae-core/ui_resource/features/modifiers_holder.hpp>

using namespace yas;
using namespace yas::ae;

keyboard::keyboard(std::shared_ptr<ui::event_manager> const &manager, modifiers_holder *modifiers_holder)
    : _event_manager(manager),
      _modifiers_holder(modifiers_holder),
      _key_notifier(observing::notifier<ae::key>::make_shared()) {
    this->_event_manager
        ->observe([this](std::shared_ptr<ui::event> const &event) {
            switch (event->type()) {
                case ui::event_type::key: {
                    auto const phase = event->phase();
                    if (phase == ui::event_phase::began || phase == ui::event_phase::changed) {
                        auto const key_event = event->get<ui::key>();
                        if (auto const key = keyboard_utils::to_key(key_event, this->_modifiers_holder->modifiers())) {
                            this->_key_notifier->notify(key.value());
                        }
                    }
                } break;

                case ui::event_type::modifier:
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

observing::endable keyboard::observe(std::function<void(ae::key const &)> &&handler) {
    return this->_key_notifier->observe(std::move(handler));
}
