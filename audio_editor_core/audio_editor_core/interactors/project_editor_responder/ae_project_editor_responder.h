//
//  ae_project_editor_responder.h
//

#pragma once

#include <audio_editor_core/ae_responder.h>

namespace yas::ae {
class project_editor;
class playing_toggler;

struct project_editor_responder final : responder {
    [[nodiscard]] static std::shared_ptr<project_editor_responder> make_shared(
        std::shared_ptr<project_editor> const &, std::shared_ptr<playing_toggler> const &);

    std::optional<ae::action> to_action(ae::key const &) override;
    void handle_action(ae::action const &) override;
    [[nodiscard]] responding responding_to_action(ae::action const &) override;

   private:
    identifier const _responder_id;
    std::weak_ptr<project_editor> const _editor;
    std::weak_ptr<playing_toggler> const _playing_toggler;

    project_editor_responder(std::shared_ptr<project_editor> const &, std::shared_ptr<playing_toggler> const &);
};
}  // namespace yas::ae
