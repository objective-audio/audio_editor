//
//  ae_project.h
//

#pragma once

#include <audio_editor_core/ae_ptr.h>

namespace yas::ae {
struct project {
    static project_ptr make_shared();

   private:
    project();

    project(project const &) = delete;
    project(project &&) = delete;
    project &operator=(project const &) = delete;
    project &operator=(project &&) = delete;
};
}  // namespace yas::ae
