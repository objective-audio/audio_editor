//
//  ae_project.h
//

#pragma once

#include <audio_editor_core/ae_ptr.h>
#include <cpp_utils/yas_url.h>

namespace yas::ae {
struct project {
    url const &file_url() const;

    bool requestClose();

    static project_ptr make_shared(url const &);

   private:
    url const _file_url;

    project(url const &);

    project(project const &) = delete;
    project(project &&) = delete;
    project &operator=(project const &) = delete;
    project &operator=(project &&) = delete;
};
}  // namespace yas::ae
