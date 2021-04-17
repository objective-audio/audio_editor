//
//  ae_app.h
//

#pragma once

#include <audio_editor_core/ae_ptr.h>
#include <cpp_utils/yas_url.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct app {
    void add_project(url const &file_url);

    static app_ptr make_shared();

   private:
    observing::vector::holder_ptr<project_ptr> const _projects = observing::vector::holder<project_ptr>::make_shared();

    app();

    app(app const &) = delete;
    app(app &&) = delete;
    app &operator=(app const &) = delete;
    app &operator=(app &&) = delete;
};
}  // namespace yas::ae
