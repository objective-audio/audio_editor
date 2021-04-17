//
//  ae_app.h
//

#pragma once

#include <audio_editor_core/ae_ptr.h>

namespace yas::ae {
struct app {
    static app_ptr make_shared();

   private:
    app();

    app(app const &) = delete;
    app(app &&) = delete;
    app &operator=(app const &) = delete;
    app &operator=(app &&) = delete;
};
}  // namespace yas::ae
