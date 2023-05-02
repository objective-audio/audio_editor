//
//  ae_settings_closer.cpp
//

#include "ae_settings_closer.hpp"

#include <audio_editor_core/ae_settings_lifecycle.hpp>

using namespace yas;
using namespace yas::ae;

settings_closer::settings_closer(window_lifetime_id const &lifetime_id, settings_lifecycle *lifecycle)
    : _lifetime_id(lifetime_id), _lifecycle(lifecycle) {
}

bool settings_closer::can_close() const {
    // TODO: 閉じれない場合の処理を追加する。レスポンダチェーンで再帰的にチェックが必要そう？
    return this->_lifecycle != nullptr;
}

void settings_closer::close_if_needed() {
    if (!this->can_close()) {
        return;
    }

    this->_lifecycle->remove_lifetime(this->_lifetime_id);
    this->_lifecycle = nullptr;
}
