//
//  ae_app.h
//

#pragma once

#include <audio_editor_core/ae_ptr.h>
#include <cpp_utils/yas_url.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct app {
    using projects_t = observing::map::holder<uintptr_t, project_ptr>;

    void add_project(url const &file_url);
    std::map<uintptr_t, project_ptr> const &projects() const;
    observing::syncable observe_projects(observing::caller<projects_t::event>::handler_f &&);

    static app_ptr make_shared();

   private:
    using projects_ptr_t = std::shared_ptr<projects_t>;
    projects_ptr_t const _projects = projects_t::make_shared();
    std::unordered_map<uintptr_t, observing::cancellable_ptr> _cancellers;

    app();

    app(app const &) = delete;
    app(app &&) = delete;
    app &operator=(app const &) = delete;
    app &operator=(app &&) = delete;
};
}  // namespace yas::ae
