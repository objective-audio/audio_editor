//
//  ae_app.h
//

#pragma once

#include <audio_editor_core/ae_app_dependency.h>

namespace yas::ae {
struct app final {
    enum class projects_event_type {
        inserted,
        erased,
    };

    struct projects_event {
        projects_event_type type;
        app_project_interface_ptr project;
    };

    using projects_map_t =
        observing::map::holder<uintptr_t, std::pair<app_project_interface_ptr, observing::cancellable_ptr>>;

    app_project_interface_ptr add_project(url const &file_url);
    [[nodiscard]] std::vector<app_project_interface_ptr> projects() const;
    [[nodiscard]] observing::syncable observe_project(std::function<void(projects_event const &)> &&);

    [[nodiscard]] static app_ptr make_shared();
    [[nodiscard]] static app_ptr make_shared(app_factory_interface_ptr const &);

   private:
    app_factory_interface_ptr const _factory;

    using projects_ptr_t = std::shared_ptr<projects_map_t>;
    projects_ptr_t const _projects = projects_map_t::make_shared();

    app(app_factory_interface_ptr const &);

    app(app const &) = delete;
    app(app &&) = delete;
    app &operator=(app const &) = delete;
    app &operator=(app &&) = delete;
};
}  // namespace yas::ae
