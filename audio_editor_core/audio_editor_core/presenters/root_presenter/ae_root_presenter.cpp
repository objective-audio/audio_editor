//
//  ae_root_presenter.cpp
//

#include "ae_root_presenter.h"

#include <audio_editor_core/ae_hierarchy.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<root_presenter> root_presenter::make_shared() {
    return std::make_shared<root_presenter>();
}

root_presenter::root_presenter() {
}
