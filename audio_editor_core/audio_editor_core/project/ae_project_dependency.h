//
//  ae_project_dependency.h
//

#pragma once

#include <cpp_utils/yas_url.h>

namespace yas::ae {
struct project_url_interface {
    virtual ~project_url_interface() = default;

    virtual url const &root_directory() const = 0;
    virtual url editing_file() const = 0;
};

struct project_file_importer_interface {
    virtual ~project_file_importer_interface() = default;

    virtual void import(url const &src_url, url const &dst_url, std::function<void(bool const)> &&completion) = 0;
};
}  // namespace yas::ae
