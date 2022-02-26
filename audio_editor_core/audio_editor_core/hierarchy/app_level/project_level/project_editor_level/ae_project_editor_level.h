//
//  ae_project_editor_level.h
//

#pragma once

#include <memory>

namespace yas::ae {
class file_info;
class timing;
class nudging;
class project_editor;

struct project_editor_level final {
    [[nodiscard]] static std::shared_ptr<project_editor_level> make_shared(std::string const &identifier,
                                                                           file_info const &);

    std::shared_ptr<timing> const timing;
    std::shared_ptr<nudging> const nudging;
    std::shared_ptr<project_editor> const editor;

   private:
    project_editor_level(std::string const &identifier, file_info const &);
};
}  // namespace yas::ae
