//
//  ae_project_setup_dialog_lifetime.h
//

#pragma once

#include <memory>

namespace yas::ae {
class project_format_setup;
class project_setup;

struct project_setup_dialog_lifetime final {
    [[nodiscard]] static std::shared_ptr<project_setup_dialog_lifetime> make_shared();

    project_setup_dialog_lifetime(std::shared_ptr<project_format_setup> const &,
                                  std::shared_ptr<project_setup> const &);

    std::shared_ptr<project_format_setup> const format_setup;
    std::shared_ptr<project_setup> const setup;
};
}  // namespace yas::ae
