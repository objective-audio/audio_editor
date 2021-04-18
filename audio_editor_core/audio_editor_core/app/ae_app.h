//
//  ae_app.h
//

#pragma once

#include <audio_editor_core/ae_ptr.h>
#include <cpp_utils/yas_url.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct app {
    using projects_map_t = observing::map::holder<uintptr_t, std::pair<project_ptr, observing::cancellable_ptr>>;

    void add_project(url const &file_url);
    std::vector<project_ptr> projects() const;
    observing::syncable observe_projects(observing::caller<projects_map_t::event>::handler_f &&);

    static app_ptr make_shared();

   private:
    using projects_ptr_t = std::shared_ptr<projects_map_t>;
    projects_ptr_t const _projects = projects_map_t::make_shared();

    app();

    app(app const &) = delete;
    app(app &&) = delete;
    app &operator=(app const &) = delete;
    app &operator=(app &&) = delete;
};
}  // namespace yas::ae
