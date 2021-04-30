//
//  ae_project_url.h
//

#pragma once

#include <audio_editor_core/ae_project_dependency.h>

namespace yas::ae {
struct project_url final : project_url_interface {
    url const &root_directory() const override;
    url editing_file() const override;

    static std::shared_ptr<project_url> make_shared(url const &root);

   private:
    url _root;

    project_url(url const &root);
};
}  // namespace yas::ae
