//
//  ae_project_editor_level.h
//

#pragma once

#include <audio_editor_core/ae_file_info.h>

#include <memory>
#include <string>

namespace yas::ae {
class file_info;
class timing;
class nudging;
class project_editor;

struct project_editor_level final {
    [[nodiscard]] static std::shared_ptr<project_editor_level> make_shared(std::string const &identifier,
                                                                           file_info const &);

    std::string const identifier;
    file_info const file_info;

    std::shared_ptr<timing> const timing;
    std::shared_ptr<nudging> const nudging;
    std::shared_ptr<project_editor> const editor;

   private:
    project_editor_level(std::string const &identifier, ae::file_info const &);
};
}  // namespace yas::ae
