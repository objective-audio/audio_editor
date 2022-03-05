//
//  ae_responder.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <cpp_utils/yas_identifier.h>

namespace yas::ae {
enum class responding {
    accepting,
    blocking,
    fallthrough,
};

struct responder {
    virtual ~responder() = default;

    virtual identifier responder_id() = 0;
    virtual void handle_action(ae::action const &) = 0;
    virtual responding responding_to_action(ae::action const &) = 0;
};
}  // namespace yas::ae
