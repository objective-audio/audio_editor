//
//  ae_project_closer.cpp
//

#include "ae_project_closer.h"

#include <ae-core/app/lifecycles/ae_project_lifecycle.h>

#include <ae-core/app/lifecycles/ae_project_settings_lifecycle.hpp>

using namespace yas;
using namespace yas::ae;

project_closer::project_closer(project_lifetime_id const &lifetime_id, project_lifecycle *project_lifecycle,
                               project_settings_lifecycle *project_settings_lifecycle)
    : _lifetime_id(lifetime_id),
      _project_lifecycle(project_lifecycle),
      _project_settings_lifecycle(project_settings_lifecycle) {
}

bool project_closer::can_close() const {
    // TODO: 閉じれない場合の処理を追加する。レスポンダチェーンで再帰的にチェックが必要そう？
    return this->_project_lifecycle != nullptr;
}

void project_closer::close_if_needed() {
    if (!this->can_close()) {
        return;
    }

    this->_project_lifecycle->remove_lifetime(this->_lifetime_id);
    this->_project_lifecycle = nullptr;

    this->_project_settings_lifecycle->remove_lifetime(this->_lifetime_id);
    this->_project_settings_lifecycle = nullptr;
}
