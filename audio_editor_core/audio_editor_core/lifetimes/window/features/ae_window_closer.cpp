//
//  ae_window_closer.cpp
//

#include "ae_window_closer.h"

#include <audio_editor_core/ae_window_lifecycle.h>

#include <audio_editor_core/ae_project_settings_lifecycle.hpp>

using namespace yas;
using namespace yas::ae;

window_closer::window_closer(project_lifetime_id const &lifetime_id, window_lifecycle *window_lifecycle,
                             project_settings_lifecycle *project_settings_lifecycle)
    : _lifetime_id(lifetime_id),
      _window_lifecycle(window_lifecycle),
      _project_settings_lifecycle(project_settings_lifecycle) {
}

bool window_closer::can_close() const {
    // TODO: 閉じれない場合の処理を追加する。レスポンダチェーンで再帰的にチェックが必要そう？
    return this->_window_lifecycle != nullptr;
}

void window_closer::close_if_needed() {
    if (!this->can_close()) {
        return;
    }

    this->_window_lifecycle->remove_lifetime(this->_lifetime_id);
    this->_window_lifecycle = nullptr;

    this->_project_settings_lifecycle->remove_lifetime(this->_lifetime_id);
    this->_project_settings_lifecycle = nullptr;
}
