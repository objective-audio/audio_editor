//
//  ae_project_editor_level.h
//

#pragma once

#include <memory>

namespace yas::ae {
class project_editor;

struct project_editor_level final {
    [[nodiscard]] static std::shared_ptr<project_editor_level> make_shared(std::shared_ptr<project_editor> const &);

    std::shared_ptr<project_editor> const editor;

   private:
    project_editor_level(std::shared_ptr<project_editor> const &);
};
}  // namespace yas::ae
