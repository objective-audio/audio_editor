//
//  ae_time_presenter.h
//

#pragma once

#include <audio_editor_core/ae_time_presenter_dependency.h>

#include <memory>
#include <string>

namespace yas::ae {
struct time_presenter final {
    [[nodiscard]] static std::shared_ptr<time_presenter> make_shared(std::string const project_id);

    [[nodiscard]] std::string time_text() const;

   private:
    std::weak_ptr<project_editor_for_time_presenter> _project_editor;

    time_presenter(std::shared_ptr<project_editor_for_time_presenter> const &);
};
}  // namespace yas::ae
