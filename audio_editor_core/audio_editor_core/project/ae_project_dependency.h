//
//  ae_project_dependency.h
//

#pragma once

#include <audio_editor_core/ae_file_importer_types.h>
#include <audio_editor_core/ae_file_loader_types.h>
#include <cpp_utils/yas_url.h>

#include <optional>

namespace yas::ae {
struct project_url_interface {
    virtual ~project_url_interface() = default;

    virtual url const &root_directory() const = 0;
    virtual url editing_file() const = 0;
};

struct project_file_importer_interface {
    virtual ~project_file_importer_interface() = default;

    virtual void import(file_importing_context &&) = 0;
    virtual void cancel(std::string const &) = 0;
};

struct project_file_loader_interface {
    virtual ~project_file_loader_interface() = default;

    virtual std::optional<file_info> load_file_info(url const &) const = 0;
};
}  // namespace yas::ae
