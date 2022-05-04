//
//  ae_time_editor_level.h
//

#pragma once

#include <cpp_utils/yas_identifier.h>

#include <memory>
#include <optional>
#include <string>

namespace yas::ae {
class number_components;
class time_editor;
class time_editor_responder;
class time_editor_status;

struct time_editor_level final {
    [[nodiscard]] static std::shared_ptr<time_editor_level> make_shared(std::string const &project_id,
                                                                        number_components const &,
                                                                        std::optional<std::size_t> const unit_idx);

    identifier const instance_id;
    std::string const project_id;
    std::shared_ptr<time_editor_status> const status;
    std::shared_ptr<time_editor> const editor;
    std::shared_ptr<time_editor_responder> const responder;

   private:
    time_editor_level(std::string const &project_id, number_components const &,
                      std::optional<std::size_t> const unit_idx);
};
}  // namespace yas::ae
