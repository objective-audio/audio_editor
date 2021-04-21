//
//  ae_app_delegate_presenter.h
//

#pragma once

#include <audio_editor_core/ae_ptr.h>

namespace yas::ae {
struct app_delegate_presenter {
    app_delegate_presenter();
    app_delegate_presenter(app_ptr const &);

   private:
    app_ptr _app;
};
}  // namespace yas::ae
