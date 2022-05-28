//
//  ae_project_setup_dialog_level.h
//

#pragma once

#include <memory>

namespace yas::ae {
class project_format_setup;
class project_setup;

struct project_setup_dialog_level final {
    [[nodiscard]] static std::shared_ptr<project_setup_dialog_level> make_shared();

    project_setup_dialog_level(std::shared_ptr<project_format_setup> const &, std::shared_ptr<project_setup> const &);

    std::shared_ptr<project_format_setup> const format_setup;
    std::shared_ptr<project_setup> const setup;
};
}  // namespace yas::ae
