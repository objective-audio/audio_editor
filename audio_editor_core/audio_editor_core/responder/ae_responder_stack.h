//
//  ae_responder_stack.h
//

#pragma once

#include <audio_editor_core/ae_project_launcher_dependency.h>
#include <audio_editor_core/ae_responder.h>

#include <deque>
#include <memory>

namespace yas::ae {
struct responder_stack final : responder_stack_for_project {
    [[nodiscard]] static std::shared_ptr<responder_stack> make_shared();

    std::optional<ae::action> to_action(ae::key const &);
    void handle_action(ae::action const &);
    [[nodiscard]] responding responding_to_action(ae::action const &);

    void push_responder(identifier const &, std::shared_ptr<ae::responder> const &) override;
    void pop_responder(identifier const &);

   private:
    std::deque<std::pair<identifier, std::weak_ptr<ae::responder>>> _responders;

    responder_stack();
};
}  // namespace yas::ae
