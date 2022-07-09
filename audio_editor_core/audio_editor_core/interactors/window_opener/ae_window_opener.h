//
//  ae_window_opener.h
//

#pragma once

#include <cpp_utils/yas_url.h>

#include <memory>

namespace yas::ae {
class file_info_loader;
class window_lifecycle;
class project_format;

struct window_opener final {
    [[nodiscard]] static std::shared_ptr<window_opener> make_shared(file_info_loader const *, window_lifecycle *);

    window_opener(file_info_loader const *, window_lifecycle *);

    void open(project_format const &, url const &project_url);

   private:
    file_info_loader const *const _file_info_loader;
    window_lifecycle *const _window_lifecycle;
};
}  // namespace yas::ae
