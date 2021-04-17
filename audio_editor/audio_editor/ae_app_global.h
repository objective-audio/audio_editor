//
//  ae_app_global.h
//

#pragma once

#include <audio_editor_core/audio_editor_core_umbrella.h>

namespace yas::ae {
app_ptr const app_global = app::make_shared();
}
