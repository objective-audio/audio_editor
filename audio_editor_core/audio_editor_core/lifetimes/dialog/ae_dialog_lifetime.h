//
//  ae_dialog_lifetime.h
//

#pragma once

#include <audio_editor_core/ae_dialog_content.h>
#include <audio_editor_core/ae_project_sub_lifetime_id.h>

#include <memory>

namespace yas::ae {
struct dialog_lifetime final {
    dialog_lifetime(project_sub_lifetime_id const &, dialog_content const);

    project_sub_lifetime_id const lifetime_id;
    dialog_content const content;

   private:
    dialog_lifetime(dialog_lifetime const &) = delete;
    dialog_lifetime(dialog_lifetime &&) = delete;
    dialog_lifetime &operator=(dialog_lifetime const &) = delete;
    dialog_lifetime &operator=(dialog_lifetime &&) = delete;
};
}  // namespace yas::ae
