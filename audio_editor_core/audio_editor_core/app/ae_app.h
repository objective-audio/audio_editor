//
//  ae_app.h
//

#pragma once

#include <audio_editor_core/ae_app_dependency.h>
#include <audio_editor_core/ae_app_types.h>

namespace yas::ae {
class project_pool;
class system_url;
class file_importer;
class file_loader;
class color;

struct app final {
    [[nodiscard]] static std::shared_ptr<app> make_shared();
    [[nodiscard]] static std::shared_ptr<app> global();

    [[nodiscard]] std::shared_ptr<project_pool> const &project_pool() const;
    [[nodiscard]] std::shared_ptr<system_url> const &system_url() const;
    [[nodiscard]] std::shared_ptr<file_importer> const &file_importer() const;
    [[nodiscard]] std::shared_ptr<file_loader> const &file_loader() const;
    [[nodiscard]] std::shared_ptr<color> const &color() const;

   private:
    worker_ptr const _worker;
    std::shared_ptr<ae::project_pool> const _project_pool;
    std::shared_ptr<ae::system_url> const _system_url;
    std::shared_ptr<ae::file_importer> const _file_importer;
    std::shared_ptr<ae::file_loader> const _file_loader;
    std::shared_ptr<ae::color> const _color;

    app();

    app(app const &) = delete;
    app(app &&) = delete;
    app &operator=(app const &) = delete;
    app &operator=(app &&) = delete;
};
}  // namespace yas::ae
