//
//  ae_root_presenter.h
//

#pragma once

#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct root_presenter final {
    [[nodiscard]] static std::shared_ptr<root_presenter> make_shared();

    root_presenter();

   private:
    root_presenter(root_presenter const &) = delete;
    root_presenter(root_presenter &&) = delete;
    root_presenter &operator=(root_presenter const &) = delete;
    root_presenter &operator=(root_presenter &&) = delete;
};
}  // namespace yas::ae
