//
//  ae_system_url.h
//

#pragma once

#include <audio_editor_core/ae_project_id.h>
#include <cpp_utils/yas_url.h>

namespace yas::ae {
struct system_url final {
    [[nodiscard]] url document_directory() const;
    [[nodiscard]] url app_directory() const;
    [[nodiscard]] url playing_directory() const;
    [[nodiscard]] url project_directory(project_id const &project_id) const;

    [[nodiscard]] static std::shared_ptr<system_url> make_shared();

   private:
    system_url();

    system_url(system_url const &) = delete;
    system_url(system_url &&) = delete;
    system_url &operator=(system_url const &) = delete;
    system_url &operator=(system_url &&) = delete;

    void _log_app_directory() const;
};
}  // namespace yas::ae
