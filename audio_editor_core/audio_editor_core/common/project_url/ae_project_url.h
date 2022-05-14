//
//  ae_project_url.h
//

#pragma once

#include <cpp_utils/yas_url.h>

namespace yas::ae {
struct project_url final {
    url const &root_directory() const;
    url editing_file() const;
    url playing_directory() const;
    url db_file() const;

    static std::shared_ptr<project_url> make_shared(url const &root);

   private:
    url _root;

    project_url(url const &root);

    project_url(project_url const &) = delete;
    project_url(project_url &&) = delete;
    project_url &operator=(project_url const &) = delete;
    project_url &operator=(project_url &&) = delete;
};
}  // namespace yas::ae
