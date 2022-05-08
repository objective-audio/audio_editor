//
//  ae_pasteboard.h
//

#pragma once

#include <audio_editor_core/ae_pasteboard_types.h>
#include <audio_editor_core/ae_project_editor_dependency.h>

namespace yas::ae {
struct pasteboard final : pasteboard_for_project_editor {
    [[nodiscard]] static std::shared_ptr<pasteboard> make_shared();

    pasteboard();

    [[nodiscard]] std::optional<pasting_file_module> file_module() const override;
    void set_file_module(pasting_file_module const &) override;

    [[nodiscard]] std::string const &data() const override;
    void revert_data(std::string const &) override;

    [[nodiscard]] observing::syncable observe_event(std::function<void(pasteboard_event const &)> &&) override;

   private:
    std::string _data = "";
    observing::fetcher_ptr<pasteboard_event> const _event_fetcher;
};
}  // namespace yas::ae
