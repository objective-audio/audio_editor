//
//  ae_app_delegate_presenter.h
//

#pragma once

#include <audio_editor_core/ae_ptr.h>

namespace yas::ae {
struct app_delegate_presenter {
    static std::shared_ptr<app_delegate_presenter> make_shared();

   private:
    app_delegate_presenter();

    app_delegate_presenter(app_delegate_presenter const &) = delete;
    app_delegate_presenter(app_delegate_presenter &&) = delete;
    app_delegate_presenter &operator=(app_delegate_presenter const &) = delete;
    app_delegate_presenter &operator=(app_delegate_presenter &&) = delete;
};
}  // namespace yas::ae
