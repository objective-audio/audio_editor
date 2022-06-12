//
//  ae_window_closer.cpp
//

#include "ae_window_closer.h"

#include <audio_editor_core/ae_window_lifecycle.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<window_closer> window_closer::make_shared(project_id const &project_id,
                                                          window_lifecycle *window_lifecycle) {
    return std::make_shared<window_closer>(project_id, window_lifecycle);
}

window_closer::window_closer(project_id const &project_id, window_lifecycle *window_lifecycle)
    : _project_id(project_id), _window_lifecycle(window_lifecycle) {
}

bool window_closer::can_close() const {
    // TODO: 閉じれない場合の処理を追加する。レスポンダチェーンで再帰的にチェックが必要そう
    return true;
}

void window_closer::close_if_needed() {
    if (!this->can_close()) {
        return;
    }

    if (!this->_window_lifecycle) {
        return;
    }

    this->_window_lifecycle->remove_lifetime(this->_project_id);
    this->_window_lifecycle = nullptr;
}
