//
//  ae_responder.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_keyboard_types.h>
#include <cpp_utils/yas_identifier.h>

#include <optional>

namespace yas::ae {
enum class responding {
    accepting,
    blocking,
    fallthrough,
};

struct responder {
    virtual ~responder() = default;

    virtual std::optional<ae::action> to_action(ae::key const &) = 0;
    virtual void handle_action(ae::action const &) = 0;
    [[nodiscard]] virtual responding responding_to_action(ae::action const &) = 0;
};
}  // namespace yas::ae
