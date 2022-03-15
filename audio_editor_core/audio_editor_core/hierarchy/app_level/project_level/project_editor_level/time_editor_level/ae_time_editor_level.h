//
//  ae_time_editor_level.h
//

#pragma once

#include <memory>
#include <optional>
#include <string>

namespace yas::ae {
class number_components;
class time_editor;

struct time_editor_level final {
    [[nodiscard]] static std::shared_ptr<time_editor_level> make_shared(std::string const &project_id,
                                                                        number_components const &,
                                                                        std::optional<std::size_t> const unit_idx);

    std::string const project_id;
    std::shared_ptr<time_editor> const time_editor;

   private:
    time_editor_level(std::string const &project_id, number_components const &,
                      std::optional<std::size_t> const unit_idx);
};
}  // namespace yas::ae
